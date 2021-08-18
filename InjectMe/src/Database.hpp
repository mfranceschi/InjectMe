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
      ProviderFct<void> providerFct = nullptr;
      Injected<void> value = nullptr;
    };

    class Database {
     public:
      static Database& getInstance();

      Injected<void> getForType(const std::type_index& typeIndex);

      static void makeInstanceForType(TypeData& typeData);

      bool knowsType(const std::type_index& typeIndex) const;

      void configureForType(
          const std::type_index& typeIndex, const ProviderFct<void>& providerFunction);

      ~Database() = default;
      Database(const Database&) = delete;
      Database& operator=(const Database&) = delete;
      Database(Database&&) = delete;
      Database& operator=(Database&&) = delete;

     private:
      Database() = default;
      std::map<std::type_index, TypeData> mapTypesToData;
    };
  }  // namespace InjectMe
}  // namespace mf
