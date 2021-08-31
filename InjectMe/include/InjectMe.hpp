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

    namespace internals
    {
      template <typename T>
      Deleter makeDefaultDeleter();

      template <typename T>
      ProviderFct<T> makeDefaultProvider();

      void* injectForTypeOrThrow(const std::type_info&);

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
        Deleter castedDeleter = [deleter](void* pointer) {
          deleter(static_cast<T*>(pointer));
        };
        configureWithValueNonTyped(value, castedDeleter, typeid(T));
      }

      void configureWithProviderNonTyped(
          const ProviderFct<void>& provider, const DeleterFct<void>& deleter, const std::type_info& typeInfo);
      void configureWithValueNonTyped(
          void* value, const DeleterFct<void>& deleter, const std::type_info& typeInfo);

      template <typename T>
      class ConfiguratorBase {
       public:
        virtual void done() = 0;
        ~ConfiguratorBase() = default;

       protected:
        DeleterFct<T> deleter = std::default_delete<T>();
        T* value = nullptr;
      };

      template <typename T>
      class ConfiguratorWithProvider : public ConfiguratorBase<T> {
       public:
        ConfiguratorWithProvider& setDeleter(const DeleterFct<T>& newDeleter) {
          this->deleter = newDeleter;
          return *this;
        }
        ConfiguratorWithProvider& setProvider(
            const ProviderFct<T>& newProvider) { /* assert bool(newProvider) */
          provider = newProvider;
          return *this;
        }
        void done() override {
            internals::configureWithProvider(provider, this->deleter);
        }

       private:
        ProviderFct<T> provider = []() {
          return new T;
        };
      };

      template <typename T>
      class ConfiguratorWithValue : public ConfiguratorBase<T> {
       public:
        ConfiguratorWithValue<T>(T* theValue) {
          this->value = theValue;
        }
        ConfiguratorWithValue& setDeleter(const DeleterFct<T>& newDeleter) {
          this->deleter = newDeleter;
          return *this;
        }
        void done() override {
          internals::configureWithValue(this->value, this->deleter);
        }
      };
    }  // namespace internals

    class Config {
     public:
      using ConfigPtr = std::unique_ptr<Config>;

      /**
       * Creates and returns an instance.
       */
      static ConfigPtr getInstance();

      /**
       * Sets a provider function for the given type.
       * @param provider The provider callable (no argument, returns a pointer).
       * @returns this.
       * @throws std::logic_error if a provider function is already registered for the given type.
       */
      template <typename T>
      Config* add(
          const ProviderFct<T>& provider = internals::makeDefaultProvider<T>(),
          const DeleterFct<T>& deleter = internals::makeDefaultDeleter<T>());

      explicit Config(const Config&) = delete;
      explicit Config(const Config&&) = delete;
      Config& operator=(const Config&) = delete;
      Config& operator=(const Config&&) = delete;
      virtual ~Config() = default;

     protected:
      Config() = default;
      virtual void setProviderForTypeOrThrow(
          const ProviderFct<void>&, const std::type_info&, const Deleter&) = 0;
    };

    /**
     * Configures InjectMe with the given Config instance.
     * @throws std::invalid_argument if the pointer is invalid.
     * @throws std::logic_error if no provider has been set.
     */
    void configure(const Config::ConfigPtr&);

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
    Config* Config::add(const ProviderFct<T>& provider, const DeleterFct<T>& deleter) {
      static_assert(!std::is_array<T>::value, "Array types are not allowed.");
      const std::type_info& typeInfo = typeid(T);
      ProviderFct<void> castedProvider = provider;
      Deleter castedDeleter = [deleter](void* pointer) {
        deleter(static_cast<T*>(pointer));
      };

      this->setProviderForTypeOrThrow(castedProvider, typeInfo, castedDeleter);
      return this;
    }

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
