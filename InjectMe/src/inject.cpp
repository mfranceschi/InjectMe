#include <functional>
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>

#include "Database.hpp"
#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace internals
    {
      Injected<void> injectForTypeOrThrow(const std::type_info& typeInfo) {
        try {
          return Database::getInstance().getForType(std::type_index(typeInfo));
        } catch (const std::out_of_range&) {
          throw std::logic_error(
              "InjectMe::inject - we don't have a provider for the given type (TODO TYPE).");
        }
      }
    }  // namespace internals
  }    // namespace InjectMe
}  // namespace mf
