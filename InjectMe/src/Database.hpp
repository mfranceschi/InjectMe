#pragma once

#include <map>
#include <set>
#include <typeindex>

#include "InjectMe.hpp"
#include "TypeData.hpp"

namespace mf
{
  namespace InjectMe
  {
    class Database {
     public:
      static Database& getDatabase();

      void* getForType(const std::type_index& typeIndex);

      bool knowsType(const std::type_index& typeIndex) const;

      void configureForType(
          const std::type_index& typeIndex,
          const ProviderFct<void>& providerFunction,
          const Deleter& deleterFunction);

      void reset();

      ~Database();
      Database(const Database&) = delete;
      Database& operator=(const Database&) = delete;
      Database(Database&&) = delete;
      Database& operator=(Database&&) = delete;

      friend class DatabaseInstanceInsertion;

     private:
      Database() = default;

      std::map<std::type_index, TypeData> mapTypesToData{};
      std::set<std::type_index> typesInConstruction{};
    };
  }  // namespace InjectMe
}  // namespace mf
