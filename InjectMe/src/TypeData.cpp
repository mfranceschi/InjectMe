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
        : typeIndex(typeIndex), uniquePtr(nullptr, deleterFct) {
    }

    TypeDataPtr TypeData::makeWithProvider(
        const std::type_index& typeIndex,
        const ProviderFct<void>& providerFct,
        const Deleter& deleterFct) {
      // Note: we cannot use sd::make_shared because the constructors are private.
      return std::shared_ptr<TypeData>(
          new TypeDataWithProvider(typeIndex, providerFct, deleterFct));
    }

    TypeDataPtr TypeData::makeWithValue(
        const std::type_index& typeIndex, void* value, const Deleter& deleterFct) {
      // Note: we cannot use sd::make_shared because the constructors are private.
      return std::shared_ptr<TypeData>(new TypeDataWithValue(typeIndex, value, deleterFct));
    }

    bool TypeData::hasValue() const {
      return bool(uniquePtr);
    }

    TypeDataWithProvider::TypeDataWithProvider(
        const std::type_index& typeIndex,
        const ProviderFct<void>& providerFct,
        const Deleter& deleterFct)
        : TypeData(typeIndex, deleterFct), providerFct(providerFct) {
    }

    void* TypeDataWithProvider::getValueAndMakeIfNeeded() {
      if (!uniquePtr) {
        DatabaseInstanceInsertion dii(getTypeIndex());
        uniquePtr.reset(providerFct());
      }
      return uniquePtr.get();
    }

    TypeDataWithValue::TypeDataWithValue(
        const std::type_index& typeIndex, void* value, const Deleter& deleterFct)
        : TypeData(typeIndex, deleterFct) {
      uniquePtr.reset(value);
    }

    void* TypeDataWithValue::getValueAndMakeIfNeeded() {
      return uniquePtr.get();
    }
  }  // namespace InjectMe
}  // namespace mf
