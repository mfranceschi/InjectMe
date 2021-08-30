#include "TypeData.hpp"

#include <map>
#include <set>
#include <typeindex>

#include "DatabaseInstanceInsertion.hpp"
#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    const std::type_index& TypeData::getTypeIndex() const {
      return typeIndex;
    }

    TypeData::TypeData(const std::type_index& typeIndex, const Deleter& deleterFct)
        : typeIndex(typeIndex), deleterFct(deleterFct) {
    }

    TypeData::~TypeData() {
      if (value != nullptr) {
        deleterFct(value);
      }
    }

    TypeDataPtr TypeData::make(
        const std::type_index& typeIndex,
        const ProviderFct<void>& providerFct,
        const Deleter& deleterFct) {
      // Note: we cannot use sd::make_shared because the constructors are private.
      return std::shared_ptr<TypeData>(
          new TypeDataWithProvider(typeIndex, providerFct, deleterFct));
    }

    bool TypeData::hasValue() const {
      return value != nullptr;
    }

    TypeDataWithProvider::TypeDataWithProvider(
        const std::type_index& typeIndex,
        const ProviderFct<void>& providerFct,
        const Deleter& deleterFct)
        : TypeData(typeIndex, deleterFct), providerFct(providerFct) {
    }

    void* TypeDataWithProvider::getValueAndMakeIfNeeded() {
      if (!value) {
        DatabaseInstanceInsertion dii(getTypeIndex());
        value = providerFct();
      }
      return value;
    }
  }  // namespace InjectMe
}  // namespace mf
