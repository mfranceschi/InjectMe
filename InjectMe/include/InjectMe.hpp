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

    class Config {
     public:
      template <typename T>
      using ProviderFct = std::function<std::add_pointer_t<T>(void)>;

      using ConfigPtr = std::unique_ptr<Config>;

      /**
       * Creates and returns an instance.
       */
      static ConfigPtr getInstance();

      /**
       * Sets a provider function for the given type.
       * @param provider The provider callable (no argument, returns a pointer).
       * @returns this.
       * @throws TODO if a provider function is already registered for the given type.
       */
      template <typename T>
      Config* add(const ProviderFct<T>& provider);

     protected:
      virtual void setProviderForType(const ProviderFct<void>&, const std::type_info&) = 0;
    };

    /**
     * Configures InjectMe with the given Config instance.
     * @throws std::invalid_argument if the pointer is invalid.
     * @throws std::logic_error if no provider has been set.
     */
    void configure(const Config::ConfigPtr&);

    // ----- INLINE AND TEMPLATE IMPLEMENTATIONS ----- //
    template <typename T>
    Config* Config::add(const ProviderFct<T>& provider) {
      const std::type_info& typeInfo = typeid(T);
      ProviderFct<void> castedProvider = [provider]() {
        return static_cast<void*>(provider());
      };
      this->setProviderForType(castedProvider, typeInfo);
      return this;
    }
  }  // namespace InjectMe
}  // namespace mf
