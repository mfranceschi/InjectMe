#include "Database.hpp"
#include "InjectMe.hpp"
#include "TypeData.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace internals
    {
      void configureWithProviderNonTyped(
          const ProviderFct<void>& provider,
          const DeleterFct<void>& deleter,
          const std::type_info& typeInfo) {
        auto typeData = TypeData::makeWithProvider(std::type_index(typeInfo), provider, deleter);
        Database::getDatabase().configureType(typeData);
      }
      void configureWithValueNonTyped(
          void* value, const DeleterFct<void>& deleter, const std::type_info& typeInfo) {
        auto typeData = TypeData::makeWithValue(std::type_index(typeInfo), value, deleter);
        Database::getDatabase().configureType(typeData);
      }
    }  // namespace internals
  }  // namespace InjectMe
}  // namespace mf
