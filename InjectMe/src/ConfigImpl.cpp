#include "ConfigImpl.hpp"

#include <sstream>

namespace mf
{
  namespace InjectMe
  {
    void ConfigImpl::setProviderForType(const ProviderPtr<void>& provider,
                                        const std::type_info& typeInfo) {
      auto typeIndex = std::type_index(typeInfo);
      auto itemInMapIterator = typesToProviders.find(typeIndex);
      if (itemInMapIterator == typesToProviders.cend()) {
        typesToProviders[typeIndex] = provider;
      } else {
        std::ostringstream oss;
        oss << "Unable to add once again a provider for the type"
            << typeIndex.name() << ".";
        throw std::logic_error(oss.str());
      }
    }
  }  // namespace InjectMe
}  // namespace mf
