#include "Database.hpp"

#include <functional>
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    Database& Database::getInstance() {
      static Database singleton;
      return singleton;
    }

    Injected<void> Database::getForType(const std::type_index& typeIndex) {
      TypeData& typeData = mapTypesToData.at(typeIndex);  // throws out_of_range
      if (typeData.value == nullptr) {
        makeInstanceForType(typeData);
      }
      return typeData.value;
    }

    void Database::makeInstanceForType(TypeData& typeData) {
      typeData.value = typeData.providerFct();
    }

    bool Database::knowsType(const std::type_index& typeIndex) const {
      return mapTypesToData.find(typeIndex) != mapTypesToData.cend();
    }

    void Database::configureForType(
        const std::type_index& typeIndex, const ProviderFct<void>& providerFunction,
        const Deleter& deleterFunction) {
      TypeData typeData;
      typeData.providerFct = providerFunction;
      typeData.deleterFct = deleterFunction;
      mapTypesToData.insert(std::make_pair(typeIndex, typeData));
    }

    Database::~Database() {
      for (const auto& pair : mapTypesToData) {
        const auto& allocatedValue = pair.second.value;
        if (allocatedValue != nullptr) {
          pair.second.deleterFct(allocatedValue);
        }
      }
    }
  }  // namespace InjectMe
}  // namespace mf
