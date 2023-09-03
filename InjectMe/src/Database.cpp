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

    void Database::configureType(const TypeDataPtr& typeData) {
      const auto insertResult =
          mapTypesToData.insert(std::make_pair(typeData->getTypeIndex(), typeData));
      if (!insertResult.second) {
        throwForDuplicate(typeData->getTypeIndex());
      }
    }

    void Database::provideForAll() {
      for (const auto& item : mapTypesToData) {
        item.second->getValueAndMakeIfNeeded();
      }
    }

    void Database::reset() {
      mapTypesToData.clear();
    }

    void Database::throwForDuplicate(const std::type_index& typeIndex) {
      throw exceptions::DuplicateProvider("configure", typeIndex.name());
    }

    Database::~Database() {
      reset();
    }
  }  // namespace InjectMe
}  // namespace mf
