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
      void setProviderForTypeOrThrow(
          const ProviderFct<void>& providerFunction, const std::type_info& typeInfo,
          const Deleter& deleter) override;

      bool hasProviderForType(const std::type_index&) const;
      void setProviderForType(const std::pair<ProviderFct<void>, Deleter>&, const std::type_index&);
      static void throwErrorForDuplicate(const std::type_index&);

      std::map<std::type_index, std::pair<ProviderFct<void>, Deleter>>
          mapTypesToProvidersAndDeleters;

      friend void configure(const Config::ConfigPtr&);
    };
  }  // namespace InjectMe
}  // namespace mf
