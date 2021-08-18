#include "ConfigImpl.hpp"

#include <sstream>

namespace mf
{
  namespace InjectMe
  {
    void ConfigImpl::setProviderForTypeOrThrow(
        const ProviderFct<void>& provider, const std::type_info& typeInfo) {
      auto typeIndex = std::type_index(typeInfo);

      if (hasProviderForType(typeIndex)) {
        setProviderForType(provider, typeIndex);
      } else {
        throwErrorForDuplicate(typeIndex);
      }
    }

    bool ConfigImpl::hasProviderForType(const std::type_index& typeIndex) const {
      return mapTypesToProviders.find(typeIndex) == mapTypesToProviders.cend();
    }

    void ConfigImpl::setProviderForType(
        const ProviderFct<void>& provider, const std::type_index& typeIndex) {
      mapTypesToProviders[typeIndex] = provider;
    }

    void ConfigImpl::throwErrorForDuplicate(const std::type_index& typeIndex) {
      std::ostringstream oss;
      oss << "InjectMe::Config - Unable to add once again a provider for the type '"
          << typeIndex.name() << "'.";
      throw std::logic_error(oss.str());
    }

  }  // namespace InjectMe
}  // namespace mf
