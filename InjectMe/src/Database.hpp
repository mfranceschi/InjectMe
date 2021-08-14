#pragma once

#include <functional>
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    struct TypeData {
      Config::ProviderFct<void> providerFct = nullptr;
      Injected<void> value = nullptr;
    };

    class Database {
     public:
      static Database& getInstance() {
        static Database singleton;
        return singleton;
      }

      Injected<void> getForType(const std::type_index& typeIndex) {
        TypeData& typeData = mapTypesToData.at(typeIndex);  // throws out_of_range
        if (typeData.value == nullptr) {
          makeInstanceForType(typeData);
        }
        return typeData.value;
      }

      static void makeInstanceForType(TypeData& typeData) {
        typeData.value = typeData.providerFct();
      }

     private:
      Database() = default;
      std::map<std::type_index, TypeData> mapTypesToData;
    };
  }  // namespace InjectMe
}  // namespace mf
