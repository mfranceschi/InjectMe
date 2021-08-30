#pragma once

#include <map>
#include <set>
#include <typeindex>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    class TypeData {
     public:
      void* getValueAndMakeIfNeeded();

      const std::type_index& getTypeIndex() const;
      bool hasValue() const;

      TypeData(
          const std::type_index& typeIndex,
          const ProviderFct<void>& providerFct,
          const Deleter& deleterFct);

      ~TypeData();

      TypeData(const TypeData&) = default;
      TypeData& operator=(const TypeData&) = default;

      TypeData() = delete;
      TypeData(TypeData&&) = delete;
      TypeData& operator=(TypeData&&) = delete;

     private:
      void provideValue();

      std::type_index typeIndex;
      ProviderFct<void> providerFct = nullptr;
      Deleter deleterFct = nullptr;
      void* value = nullptr;
    };
  }  // namespace InjectMe
}  // namespace mf
