#pragma once

#include <map>
#include <set>
#include <typeindex>

#include "Database.hpp"

namespace mf
{
  namespace InjectMe
  {
    class DatabaseInstanceInsertion {
     public:
      DatabaseInstanceInsertion() = delete;
      DatabaseInstanceInsertion(const std::type_index& typeIndex) : typeIndex(typeIndex) {
        auto& theSet = Database::getDatabase().typesInConstruction;
        auto insertResult = theSet.insert(typeIndex);
        if (!insertResult.second) {
          throw exceptions::ProviderRecursion("Instance generation", typeIndex.name());
        }
      }
      ~DatabaseInstanceInsertion() {
        Database::getDatabase().typesInConstruction.erase(typeIndex);
      }

     private:
      std::type_index typeIndex;
    };
  }  // namespace InjectMe
}  // namespace mf
