#include "Database.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace internals
    {
      Injected<void> injectForTypeOrThrow(const std::type_info& typeInfo) {
        return Database::getDatabase().getForType(std::type_index(typeInfo));
      }
    }  // namespace internals
  }    // namespace InjectMe
}  // namespace mf
