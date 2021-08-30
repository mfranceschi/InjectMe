#include "DatabaseInstanceInsertion.hpp"

#include <map>
#include <set>
#include <typeindex>

namespace mf
{
  namespace InjectMe
  {
    DatabaseInstanceInsertion::DatabaseInstanceInsertion(const std::type_index& typeIndex)
        : typeIndex(typeIndex) {
      if (isTypeInConstruction()) {
        throw exceptions::ProviderRecursion("Instance generation", typeIndex.name());
      }
      markAsInConstruction();
    }

    DatabaseInstanceInsertion::~DatabaseInstanceInsertion() {
      unmarkAsInConstruction();
    }

    bool DatabaseInstanceInsertion::isTypeInConstruction() const {
      return typesInConstruction.find(typeIndex) != typesInConstruction.cend();
    }

    void DatabaseInstanceInsertion::markAsInConstruction() {
      typesInConstruction.insert(typeIndex);
    }

    void DatabaseInstanceInsertion::unmarkAsInConstruction() {
      typesInConstruction.erase(typeIndex);
    }

    std::set<std::type_index> DatabaseInstanceInsertion::
        typesInConstruction{};  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  }  // namespace InjectMe
}  // namespace mf
