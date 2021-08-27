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
    void* TypeData::getInstanceAndMakeIfNeeded() {
      if (!value) {
        provideValue();
      }
      return value;
    }

    const std::type_index& TypeData::getTypeIndex() const {
      return typeIndex;
    }

    const Injected<void>& TypeData::getValue() const {
      return value;
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
  }  // namespace InjectMe
}  // namespace mf
