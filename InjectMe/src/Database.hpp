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
      void configure2(const TypeDataPtr& typeData);

      void reset();

      ~Database();
      Database(const Database&) = delete;
      Database& operator=(const Database&) = delete;
      Database(Database&&) = delete;
      Database& operator=(Database&&) = delete;

      friend class DatabaseInstanceInsertion;

     private:
      Database() = default;

      std::map<std::type_index, TypeDataPtr> mapTypesToData{};
    };
  }  // namespace InjectMe
}  // namespace mf
