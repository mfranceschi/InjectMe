#include "ConfigImpl.hpp"
#include "Database.hpp"
#include "InjectMe.hpp"
#include "TypeData.hpp"

namespace mf
{
  namespace InjectMe
  {
    void configure(const Config::ConfigPtr& configPtr) {
      if (!configPtr) {
        throw exceptions::InvalidPointer("configure", "configPtr is null");
      }

      const auto* configImplPtr = dynamic_cast<const ConfigImpl*>(configPtr.get());
      if (configImplPtr == nullptr) {
        throw exceptions::Internal("internal type error (not ConfigImpl)");
      }

      const auto& mapTypesToProvidersAndDeleters = configImplPtr->mapTypesToProvidersAndDeleters;

      Database& database = Database::getDatabase();
      for (const auto& pair : mapTypesToProvidersAndDeleters) {
        const auto& typeIndex = pair.first;

        if (database.knowsType(typeIndex)) {
          throw exceptions::DuplicateProvider("configure", typeIndex.name());
        }
      }

      for (const auto& pair : mapTypesToProvidersAndDeleters) {
        const auto& typeIndex = pair.first;
        const auto& providerFunction = pair.second.first;
        const auto& deleterFunction = pair.second.second;
        auto typeData = TypeData::makeWithProvider(typeIndex, providerFunction, deleterFunction);
        database.configure2(typeData);
      }
    }
  }  // namespace InjectMe
}  // namespace mf
