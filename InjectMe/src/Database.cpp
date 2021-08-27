#include "Database.hpp"

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    Database& Database::getDatabase() {
      static Database singleton;
      return singleton;
    }

    Injected<void> Database::getForType(const std::type_index& typeIndex) {
      TypeData& typeData = mapTypesToData.at(typeIndex);  // throws out_of_range
      return typeData.getInstanceAndMakeIfNeeded();
    }

    bool Database::knowsType(const std::type_index& typeIndex) const {
      return mapTypesToData.find(typeIndex) != mapTypesToData.cend();
    }

    void Database::configureForType(
        const std::type_index& typeIndex,
        const ProviderFct<void>& providerFunction,
        const Deleter& deleterFunction) {
      TypeData typeData(typeIndex, providerFunction, deleterFunction);
      mapTypesToData.insert(std::make_pair(typeIndex, typeData));
    }

    void Database::reset() {
      mapTypesToData.clear();
    }

    Database::~Database() {
      reset();
    }
  }  // namespace InjectMe
}  // namespace mf
