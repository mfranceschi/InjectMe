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
    void* TypeData::getValueAndMakeIfNeeded() {
      if (!value) {
        provideValue();
      }
      return value;
    }

    const std::type_index& TypeData::getTypeIndex() const {
      return typeIndex;
    }

    TypeData::TypeData(
        const std::type_index& typeIndex,
        const ProviderFct<void>& providerFct,
        const Deleter& deleterFct)
        : typeIndex(typeIndex), providerFct(providerFct), deleterFct(deleterFct) {
    }

    TypeData::~TypeData() {
      if (value != nullptr) {
        deleterFct(value);
      }
    }

    void TypeData::provideValue() {
      DatabaseInstanceInsertion dii(getTypeIndex());
      value = providerFct();
    }

    bool TypeData::hasValue() const {
      return value != nullptr;
    }
  }  // namespace InjectMe
}  // namespace mf
