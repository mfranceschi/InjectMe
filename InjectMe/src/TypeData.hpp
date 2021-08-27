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
      void* getInstanceAndMakeIfNeeded();
      void provideValue();

      const std::type_index& getTypeIndex() const;
      const Injected<void>& getValue() const;

      TypeData(
          const std::type_index& typeIndex,
          const ProviderFct<void>& proiderFct,
          const Deleter& deleterFct);

      ~TypeData();

      TypeData(const TypeData&) = default;
      TypeData& operator=(const TypeData&) = default;

      TypeData() = delete;
      TypeData(TypeData&&) = delete;
      TypeData& operator=(TypeData&&) = delete;

     private:
      std::type_index typeIndex;
      ProviderFct<void> providerFct = nullptr;
      Deleter deleterFct = nullptr;
      Injected<void> value = nullptr;
    };
  }  // namespace InjectMe
}  // namespace mf
