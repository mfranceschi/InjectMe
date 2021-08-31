#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace mf
{
  namespace InjectMe
  {
    template <typename T>
    using ProviderFct = std::function<T*(void)>;

    template <typename T>
    using DeleterFct = std::function<void(T*)>;

    using Deleter = std::function<void(void*)>;

    namespace exceptions
    {
      class Exception : public std::exception {
       public:
        Exception(
            const std::string& failingComponent,
            const std::string& errorName,
            const std::string& errorDetails);

        const char* what() const noexcept override;

       private:
        std::string failingComponent;
        std::string errorName;
        std::string errorDetails;
        std::string resultingString;
      };

      class InvalidPointer : public Exception {
       public:
        InvalidPointer(const std::string& failingComponent, const std::string& errorDetails = "");
      };

      class MissingProvider : public Exception {
       public:
        MissingProvider(const std::string& failingComponent, const char* typeName);
      };

      class DuplicateProvider : public Exception {
       public:
        DuplicateProvider(const std::string& failingComponent, const char* typeName);
      };

      class ProviderRecursion : public Exception {
       public:
        ProviderRecursion(const std::string& failingComponent, const char* typeName);
      };

      class Internal : public Exception {
       public:
        Internal(const std::string& errorDetails);
      };
    }  // namespace exceptions

    using Exc = exceptions::Exception;

    namespace internals
    {
      template <typename T>
      Deleter makeDefaultDeleter();

      template <typename T>
      ProviderFct<T> makeDefaultProvider();

      void* injectForTypeOrThrow(const std::type_info&);

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
        }
        void setDeleterInternal(const DeleterFct<T>& newDeleter) {
          deleter = newDeleter;
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
          setDeleterInternal(newDeleter);
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
          setDeleterInternal(newDeleter);
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

    /**
     * @returns a pointer to an instance of the given type (create if never queried before).
     * @throws std::logic_error if no Provider is known for that type.
     */
    template <typename T>
    T* inject();

    template <typename T>
    class Injected {
     public:
      Injected<T>() : pointer(inject<T>()) {
      }
      Injected<T>(T* pointer) : pointer(pointer) {
      }
      Injected(const Injected<T>&) = default;
      Injected(Injected<T>&&) noexcept = default;
      Injected<T>& operator=(const Injected<T>&) = default;
      Injected<T>& operator=(Injected<T>&&) noexcept = default;
      ~Injected() = default;

      T* get() {
        return pointer;
      }
      T& operator*() {
        return *pointer;
      }
      T* operator->() {
        return pointer;
      }
      operator T*() {
        return pointer;
      }

     private:
      T* const pointer;
    };

    namespace advanced
    {
      /**
       * Calls all Provider functions so that all instances are ready for all types.
       */
      void callAllProviders();

      /**
       * WARNING: ADVANCED USERS ONLY.
       * Forgets all types and providers configured and deletes all objects.
       * All Injected<T> pointers are invalidated (they point to freed memory).
       */
      void reset();
    }  // namespace advanced

    // ----- IMPLEMENTATIONS and INTERNAL FUNCTIONS ----- //
    namespace internals
    {
      template <typename T>
      Deleter makeDefaultDeleter() {
        return [](void* pointerToDelete) {
          T* castedPointer = static_cast<T*>(pointerToDelete);
          std::default_delete<T>()(castedPointer);
        };
      }

      template <typename T>
      ProviderFct<T> makeDefaultProvider() {
        return []() -> T* {
          return new T;  // NOLINT(cppcoreguidelines-owning-memory)
        };
      }
    }  // namespace internals

    template <typename T>
    T* inject() {
      return static_cast<T*>(internals::injectForTypeOrThrow(typeid(T)));
    }

    /**
     * Dummy function for testing purpose.
     * @return the sum of a and b.
     * @throws std::overflow_error or std::underflow_error.
     */
    int addIntegersOrThrow(int a, int b);

  }  // namespace InjectMe
}  // namespace mf
