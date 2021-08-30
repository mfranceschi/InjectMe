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

    void* Database::getForType(const std::type_index& typeIndex) const {
      try {
        const TypeDataPtr& typeData = mapTypesToData.at(typeIndex);  // throws out_of_range
        return typeData->getValueAndMakeIfNeeded();
      } catch (const std::out_of_range&) {
        throw exceptions::MissingProvider("inject", typeIndex.name());
      }
    }

    bool Database::knowsType(const std::type_index& typeIndex) const {
      return mapTypesToData.find(typeIndex) != mapTypesToData.cend();
    }

    void Database::configure2(const TypeDataPtr& typeData) {
      mapTypesToData.insert(std::make_pair(typeData->getTypeIndex(), typeData));
    }

    void Database::reset() {
      mapTypesToData.clear();
    }

    Database::~Database() {
      reset();
    }
  }  // namespace InjectMe
}  // namespace mf
