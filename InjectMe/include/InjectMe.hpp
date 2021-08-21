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

    using Deleter = std::function<void(void*)>;

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
      Config* add(const ProviderFct<T>& provider);

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

    template <typename T>
    using Injected = T*;

    /**
     * @returns an Injected instance for the given type (create if never queried before).
     * @throws std::logic_error if no Provider is known for that type.
     */
    template <typename T>
    Injected<T> inject();

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

      class ProviderRecursion : public Exception {};
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
      Deleter makeDeleter() {
        return [](void* pointerToDelete) {
          T* castedPointer = static_cast<T*>(pointerToDelete);
          std::default_delete<T>()(castedPointer);
        };
      }

      Injected<void> injectForTypeOrThrow(const std::type_info&);
    }  // namespace internals

    template <typename T>
    Config* Config::add(const ProviderFct<T>& provider) {
      static_assert(!std::is_array<T>::value, "Array types are not allowed.");
      const std::type_info& typeInfo = typeid(T);
      ProviderFct<void> castedProvider = provider;

      this->setProviderForTypeOrThrow(castedProvider, typeInfo, internals::makeDeleter<T>());
      return this;
    }

    template <typename T>
    Injected<T> inject() {
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
