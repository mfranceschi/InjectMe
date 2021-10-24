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

      template <typename T, std::enable_if_t<!std::is_default_constructible<T>::value, bool> = true>
      ProviderFct<T> makeDefaultProvider() {
        return []() -> T* {
          throw exceptions::MissingProvider(
              "configure (for abstract or non-constructible type)", typeid(T).name());
        };
      }

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

      template <typename T>
      class ConfiguratorBase {
       public:
        ~ConfiguratorBase() = default;
        explicit ConfiguratorBase(const ConfiguratorBase&) = delete;
        explicit ConfiguratorBase(ConfiguratorBase&&) noexcept = default;
        ConfiguratorBase& operator=(const ConfiguratorBase&) = delete;
        ConfiguratorBase& operator=(ConfiguratorBase&&) noexcept = default;

        virtual void done() = 0;

       protected:
        ConfiguratorBase<T>() = default;
        ConfiguratorBase<T>(T* theValue) : value(theValue) {
          if (value == nullptr) {
            throw exceptions::InvalidPointer(
                "configure", std::string("invalid provided value for type \"") +
                                 std::string(typeid(T).name()) + std::string("\"."));
          }
        }
        void setDeleterInternal(const DeleterFct<T>& newDeleter) {
          static auto DUMMY_DELETER = [](...) {
          };
          deleter = newDeleter ? newDeleter : DUMMY_DELETER;
        }
        T* getValue() const {
          return value;
        }
        const DeleterFct<T>& getDeleterInternal() const {
          return deleter;
        }

       private:
        DeleterFct<T> deleter = std::default_delete<T>();
        T* value = nullptr;
      };

      template <typename T>
      class ConfiguratorWithProvider : public ConfiguratorBase<T> {
       public:
        ConfiguratorWithProvider() = default;
        ConfiguratorWithProvider& setDeleter(const DeleterFct<T>& newDeleter) {
          this->setDeleterInternal(newDeleter);
          return *this;
        }
        ConfiguratorWithProvider& setProvider(const ProviderFct<T>& newProvider) {
          if (!newProvider) {
            throw exceptions::MissingProvider("configuration", typeid(T).name());
          }
          provider = newProvider;
          return *this;
        }
        void done() override {
          internals::configureWithProvider(provider, this->getDeleterInternal());
        }

       private:
        ProviderFct<T> provider = internals::makeDefaultProvider<T>();
      };

      template <typename T>
      class ConfiguratorWithValue : public ConfiguratorBase<T> {
       public:
        explicit ConfiguratorWithValue<T>(T* theValue) : ConfiguratorBase<T>(theValue) {
        }
        ConfiguratorWithValue& setDeleter(const DeleterFct<T>& newDeleter) {
          this->setDeleterInternal(newDeleter);
          return *this;
        }
        void done() override {
          internals::configureWithValue(this->getValue(), this->getDeleterInternal());
        }
      };
    }  // namespace internals

    template <typename T>
    auto configure() {
      return internals::ConfiguratorWithProvider<T>();
    }

    template <typename T>
    auto configureWithValue(T* value) {
      return internals::ConfiguratorWithValue<T>(value);
    }

    template <typename T, typename... Args>
    ProviderFct<T> makeProvider(Args... args) {
      return [=]() -> T* {
        return new T(args...);  // NOLINT(cppcoreguidelines-owning-memory)
      };
    }
  }  // namespace InjectMe
}  // namespace mf
