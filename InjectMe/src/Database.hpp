#pragma once

#include <map>
#include <typeindex>

#include "InjectMe.hpp"
#include "TypeData.hpp"

namespace mf
{
  namespace InjectMe
  {
    class Database {
     public:
      static Database& getDatabase();

      void* getForType(const std::type_index& typeIndex) const;
      bool knowsType(const std::type_index& typeIndex) const;
      void configureType(const TypeDataPtr& typeData);
      void provideForAll();
      void reset();

      ~Database();
      Database(const Database&) = delete;
      Database& operator=(const Database&) = delete;
      Database(Database&&) = delete;
      Database& operator=(Database&&) = delete;

     private:
      Database() = default;
      static void throwForDuplicate(const std::type_index& typeIndex);

      std::map<std::type_index, TypeDataPtr> mapTypesToData{};
    };
  }  // namespace InjectMe
}  // namespace mf
