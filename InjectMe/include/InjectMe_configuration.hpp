#pragma once

#include <functional>
#include <memory>
#include <typeinfo>

#include "InjectMe_exceptions.hpp"
#include "InjectMe_types.hpp"

namespace mf
{
  namespace InjectMe
  {
    using Deleter = std::function<void(void*)>;

    namespace internals
    {
      template <typename T>
      Deleter makeDefaultDeleter() {
        return [](void* pointerToDelete) {
          T* castedPointer = static_cast<T*>(pointerToDelete);
          std::default_delete<T>()(castedPointer);
        };
      }

      /**
       * @returns A provider that throws, since the type is not default-constructible.
       */
      template <typename T, std::enable_if_t<!std::is_default_constructible<T>::value, bool> = true>
      ProviderFct<T> makeDefaultProvider() {
        return []() -> T* {
          throw exceptions::MissingProvider(
              "configure (for abstract or non-default-constructible type)", typeid(T).name());
        };
      }

      /**
       * @returns A provider that uses the default constructor.
       */
      template <typename T, std::enable_if_t<std::is_default_constructible<T>::value, bool> = true>
      ProviderFct<T> makeDefaultProvider() {
        return []() -> T* {
          return new T;  // NOLINT(cppcoreguidelines-owning-memory)
        };
      }

      void configureWithProviderNonTyped(
          const ProviderFct<void>& provider,
          const DeleterFct<void>& deleter,
          const std::type_info& typeInfo);

      void configureWithValueNonTyped(
          void* value, const DeleterFct<void>& deleter, const std::type_info& typeInfo);

      template <typename T>
      void configureWithProvider(const ProviderFct<T>& provider, const DeleterFct<T>& deleter) {
        ProviderFct<void> castedProvider = provider;
        Deleter castedDeleter = [deleter](void* pointer) {
          deleter(static_cast<T*>(pointer));
        };
        configureWithProviderNonTyped(castedProvider, castedDeleter, typeid(T));
      }

      template <typename T>
      void configureWithValue(T* value, const DeleterFct<T>& deleter) {
        void* castedValue = static_cast<void*>(value);
        Deleter castedDeleter = [deleter](void* pointer) {
          deleter(static_cast<T*>(pointer));
        };
        configureWithValueNonTyped(castedValue, castedDeleter, typeid(T));
      }

      template <typename T, class ConfigClass>
      class ConfiguratorBase {
       public:
        explicit ConfiguratorBase(const ConfiguratorBase&) = delete;
        ConfiguratorBase& operator=(const ConfiguratorBase&) = delete;
        explicit ConfiguratorBase(ConfiguratorBase&&) noexcept = default;
        ConfiguratorBase& operator=(ConfiguratorBase&&) noexcept = default;
        ~ConfiguratorBase() = default;

        ConfigClass& setDeleter(const DeleterFct<T>& newDeleter) {
          static auto DUMMY_DELETER = [](...) {
          };
          deleter = (newDeleter ? newDeleter : DUMMY_DELETER);
          return *((ConfigClass*)this);
        }

        virtual void done() = 0;

       protected:
        ConfiguratorBase() = default;

        ConfiguratorBase(T* theValue) : value(theValue) {
          if (value == nullptr) {
            throw exceptions::InvalidPointer(
                "configure", std::string("invalid provided value for type \"") +
                                 std::string(typeid(T).name()) + std::string("\"."));
          }
        }

        T* getValue() const {
          return value;
        }

        const DeleterFct<T>& getDeleter() const {
          return deleter;
        }

       private:
        DeleterFct<T> deleter = std::default_delete<T>();
        T* value = nullptr;
      };

      template <typename T>
      class ConfiguratorWithProvider : public ConfiguratorBase<T, ConfiguratorWithProvider<T>> {
       public:
        ConfiguratorWithProvider() = default;

        ConfiguratorWithProvider& setProvider(const ProviderFct<T>& newProvider) {
          if (!newProvider) {
            throw exceptions::MissingProvider("configuration", typeid(T).name());
          }
          provider = newProvider;
          return *this;
        }

        void done() override {
          internals::configureWithProvider(provider, this->getDeleter());
        }

       private:
        ProviderFct<T> provider = internals::makeDefaultProvider<T>();
      };

      template <typename T>
      class ConfiguratorWithValue : public ConfiguratorBase<T, ConfiguratorWithValue<T>> {
       public:
        explicit ConfiguratorWithValue<T>(T* theValue)
            : ConfiguratorBase<T, ConfiguratorWithValue<T>>(theValue) {
        }

        void done() override {
          internals::configureWithValue(this->getValue(), this->getDeleter());
        }
      };
    }  // namespace internals

    /**
     * Uses a provider.
     * The default provider uses either a default constructor if possible, or a function that
     * throws. If needed, you can change the deleter.
     *
     * Example use:
     * <code>
     * configure<int>().setProvider(validProvider).done();
     * </code>
     */
    template <typename T>
    auto configure() {
      return internals::ConfiguratorWithProvider<T>();
    }

    /**
     * Uses a pointer that you created.
     * If needed, you can change the deleter.
     *
     * Example use:
     * <code>
     * configureWithValue<int>(validInt).setDeleter(validDeleter).done();
     * </code>
     */
    template <typename T>
    auto configureWithValue(T* value) {
      return internals::ConfiguratorWithValue<T>(value);
    }

    /**
     * Utility function to create a provider for your type.
     * The returned value is not stored until you use it in a `configure*` function.
     * Parameters are copied by value.
     */
    template <typename T, typename... Args>
    ProviderFct<T> makeProvider(Args... args) {
      return [=]() -> T* {
        return new T(args...);  // NOLINT(cppcoreguidelines-owning-memory)
      };
    }
  }  // namespace InjectMe
}  // namespace mf
