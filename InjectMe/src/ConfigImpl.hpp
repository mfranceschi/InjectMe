#include <map>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    class ConfigImpl : public Config {
     protected:
      void setProviderForType(const ProviderPtr<void>&,
                              const std::type_info&) override;
      std::map<std::type_index, ProviderPtr<void>> typesToProviders;
    };
  }  // namespace InjectMe
}  // namespace mf
