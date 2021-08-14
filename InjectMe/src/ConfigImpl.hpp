#pragma once

#include <map>
#include <typeindex>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    class ConfigImpl : public Config {
     protected:
      void setProviderForType(const ProviderFct<void>&, const std::type_info&) override;
      bool hasProviderForType(const std::type_index&) const;

      std::map<std::type_index, ProviderFct<void>> mapTypesToProviders;

      friend void configure(const Config::ConfigPtr&);
    };
  }  // namespace InjectMe
}  // namespace mf
