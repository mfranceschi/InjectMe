#include <functional>
#include <memory>
#include <type_traits>
#include <typeindex>
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
      using ProviderPtr = std::function<std::add_pointer_t<T>(void)>;

      /**
       * Creates and returns an instance.
       */
      static std::unique_ptr<Config> getInstance();

      /**
       * Registers a provider function for the given type.
       * @param provider The provider callable (no argument, returns a pointer).
       * @returns this.
       * @throws TODO if a provider function is already registered for the given
       * type.
       */
      template <typename T>
      Config* add(const ProviderPtr<T>& provider);

     protected:
      virtual void setProviderForType(const ProviderPtr<void>&,
                                      const std::type_info&) = 0;
    };

    template <typename T>
    Config* Config::add(const ProviderPtr<T>& provider) {
      const std::type_info& typeInfo = typeid(T);
      ProviderPtr<void> castedProvider = [provider]() {
        return static_cast<void*>(provider());
      };
      this->setProviderForType(castedProvider, typeInfo);
      return this;
    }
  }  // namespace InjectMe
}  // namespace mf
