#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <typeinfo>

namespace mf
{
  namespace InjectMe
  {
    /**
     * Dummy function for testing purpose.
     * @return the sum of a and b.
     * @throws std::overflow_error or std::underflow_error.
     */
    int addIntegersOrThrow(int a, int b);

    template <typename T>
    using ProviderFct = std::function<T*(void)>;

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
      virtual void setProviderForTypeOrThrow(const ProviderFct<void>&, const std::type_info&) = 0;
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

    Injected<void> injectForTypeOrThrow(const std::type_info&);

    // ----- INLINE AND TEMPLATE IMPLEMENTATIONS ----- //
    template <typename T>
    Config* Config::add(const ProviderFct<T>& provider) {
      const std::type_info& typeInfo = typeid(T);
      ProviderFct<void> castedProvider = provider;

      this->setProviderForTypeOrThrow(castedProvider, typeInfo);
      return this;
    }

    template <typename T>
    Injected<T> inject() {
      return static_cast<T*>(injectForTypeOrThrow(typeid(T)));
    }
  }  // namespace InjectMe
}  // namespace mf
