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
    Injected<void> injectForTypeOrThrow(const std::type_info& typeInfo) {
      return Database::getInstance().getForType(std::type_index(typeInfo));
    }
  }  // namespace InjectMe
}  // namespace mf
