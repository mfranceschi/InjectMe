#include "ConfigImpl.hpp"

#include <sstream>

namespace mf
{
  namespace InjectMe
  {
    void ConfigImpl::setProviderForTypeOrThrow(
        const ProviderFct<void>& provider, const std::type_info& typeInfo, const Deleter& deleter) {
      auto typeIndex = std::type_index(typeInfo);

      if (hasProviderForType(typeIndex)) {
        setProviderForType(std::make_pair(provider, deleter), typeIndex);
      } else {
        throwErrorForDuplicate(typeIndex);
      }
    }

    bool ConfigImpl::hasProviderForType(const std::type_index& typeIndex) const {
      return mapTypesToProvidersAndDeleters.find(typeIndex) ==
             mapTypesToProvidersAndDeleters.cend();
    }

    void ConfigImpl::setProviderForType(
        const std::pair<ProviderFct<void>, Deleter>& providerAndDeleter,
        const std::type_index& typeIndex) {
      mapTypesToProvidersAndDeleters[typeIndex] = providerAndDeleter;
    }

    void ConfigImpl::throwErrorForDuplicate(const std::type_index& typeIndex) {
      std::ostringstream oss;
      oss << "InjectMe::Config - Unable to add once again a provider for the type '"
          << typeIndex.name() << "'.";
      throw std::logic_error(oss.str());
    }

  }  // namespace InjectMe
}  // namespace mf
