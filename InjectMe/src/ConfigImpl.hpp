#pragma once

#include <map>
#include <typeindex>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    class ConfigImpl : public Config {
     private:
      void setProviderForTypeOrThrow(const ProviderFct<void>&, const std::type_info&) override;

      bool hasProviderForType(const std::type_index&) const;
      void setProviderForType(const ProviderFct<void>&, const std::type_index&);
      static void throwErrorForDuplicate(const std::type_index&);

      std::map<std::type_index, ProviderFct<void>> mapTypesToProviders;

      friend void configure(const Config::ConfigPtr&);
    };
  }  // namespace InjectMe
}  // namespace mf
