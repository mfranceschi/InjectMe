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
      explicit DatabaseInstanceInsertion(const std::type_index& typeIndex);

      ~DatabaseInstanceInsertion();

      DatabaseInstanceInsertion() = delete;
      DatabaseInstanceInsertion(const DatabaseInstanceInsertion&) = delete;
      DatabaseInstanceInsertion& operator=(const DatabaseInstanceInsertion&) = delete;
      DatabaseInstanceInsertion(DatabaseInstanceInsertion&&) = delete;
      DatabaseInstanceInsertion& operator=(DatabaseInstanceInsertion&&) = delete;

     private:
      bool isTypeInConstruction() const;
      void markAsInConstruction();
      void unmarkAsInConstruction();

      std::type_index typeIndex;

      static std::set<std::type_index>
          typesInConstruction;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    };
  }  // namespace InjectMe
}  // namespace mf
