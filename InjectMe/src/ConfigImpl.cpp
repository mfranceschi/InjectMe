#include "ConfigImpl.hpp"

#include <sstream>

namespace mf
{
  namespace InjectMe
  {
    void ConfigImpl::setProviderForType(const ProviderFct<void>& provider,
                                        const std::type_info& typeInfo) {
      auto typeIndex = std::type_index(typeInfo);

      if (hasProviderForType(typeIndex)) {
        mapTypesToProviders[typeIndex] = provider;
      } else {
        std::ostringstream oss;
        oss << "Unable to add once again a provider for the type '" << typeIndex.name() << "'.";
        throw std::logic_error(oss.str());
      }
    }

    bool ConfigImpl::hasProviderForType(const std::type_index& typeIndex) const {
      return mapTypesToProviders.find(typeIndex) == mapTypesToProviders.cend();
    }
  }  // namespace InjectMe
}  // namespace mf
