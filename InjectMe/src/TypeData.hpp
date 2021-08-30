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

    /**
     * @brief It can provide the unique instance for a class to be injected.
     */
    class TypeData {
     public:
      virtual void* getValueAndMakeIfNeeded() = 0;
      const std::type_index& getTypeIndex() const;
      bool hasValue() const;

      static TypeDataPtr makeWithProvider(
          const std::type_index& typeIndex,
          const ProviderFct<void>& providerFct,
          const Deleter& deleterFct);

      static TypeDataPtr makeWithValue(
          const std::type_index& typeIndex, void* value, const Deleter& deleter);

      virtual ~TypeData() = default;

      TypeData() = delete;
      TypeData(const TypeData&) = delete;
      TypeData& operator=(const TypeData&) = delete;
      TypeData(TypeData&&) = delete;
      TypeData& operator=(TypeData&&) = delete;

     protected:
      TypeData(const std::type_index& typeIndex, const Deleter& deleterFct);

      std::type_index typeIndex;
      std::unique_ptr<void, Deleter> uniquePtr;
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

    class TypeDataWithValue : public TypeData {
     public:
      TypeDataWithValue(const std::type_index& typeIndex, void* value, const Deleter& deleterFct);

      void* getValueAndMakeIfNeeded() override;
    };
  }  // namespace InjectMe
}  // namespace mf
