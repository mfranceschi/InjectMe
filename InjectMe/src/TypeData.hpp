#pragma once

#include <map>
#include <set>
#include <typeindex>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    class TypeData;
    using TypeDataPtr = std::shared_ptr<TypeData>;

    class TypeData {
     public:
      virtual void* getValueAndMakeIfNeeded() = 0;
      const std::type_index& getTypeIndex() const;
      bool hasValue() const;

      static TypeDataPtr make(
          const std::type_index& typeIndex,
          const ProviderFct<void>& providerFct,
          const Deleter& deleterFct);

      virtual ~TypeData();

      TypeData() = delete;
      TypeData(const TypeData&) = delete;
      TypeData& operator=(const TypeData&) = delete;
      TypeData(TypeData&&) = delete;
      TypeData& operator=(TypeData&&) = delete;

     protected:
      TypeData(const std::type_index& typeIndex, const Deleter& deleterFct);

      std::type_index typeIndex;
      Deleter deleterFct = nullptr;
      void* value = nullptr;
    };

    class TypeDataWithProvider : public TypeData {
     public:
      TypeDataWithProvider(
          const std::type_index& typeIndex,
          const ProviderFct<void>& providerFct,
          const Deleter& deleterFct);

      void* getValueAndMakeIfNeeded() override;

     private:
      ProviderFct<void> providerFct = nullptr;
    };
  }  // namespace InjectMe
}  // namespace mf
