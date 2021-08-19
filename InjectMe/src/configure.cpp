#include "ConfigImpl.hpp"
#include "Database.hpp"
#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    void configure(const Config::ConfigPtr& configPtr) {
      if (!configPtr) {
        throw std::invalid_argument("InjectMe::configure - configPtr is a null pointer");
      }

      const auto* configImplPtr = dynamic_cast<const ConfigImpl*>(configPtr.get());
      if (configImplPtr == nullptr) {
        throw std::invalid_argument("InjectMe::configure - internal type error");
      }

      const auto& mapTypesToProvidersAndDeleters = configImplPtr->mapTypesToProvidersAndDeleters;
      if (mapTypesToProvidersAndDeleters.empty()) {
        throw std::logic_error("InjectMe::configure - no provider has been set");
      }

      Database& database = Database::getInstance();
      for (const auto& pair : mapTypesToProvidersAndDeleters) {
        const auto& typeIndex = pair.first;

        if (database.knowsType(typeIndex)) {
          throw std::logic_error("InjectMe::configure - duplicate entry for some type");
        }
      }

      for (const auto& pair : mapTypesToProvidersAndDeleters) {
        const auto& typeIndex = pair.first;
        const auto& providerFunction = pair.second.first;
        const auto& deleterFunction = pair.second.second;

        database.configureForType(typeIndex, providerFunction);
      }
    }
  }  // namespace InjectMe
}  // namespace mf
