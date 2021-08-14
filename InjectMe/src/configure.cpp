#include "ConfigImpl.hpp"
#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    void configure(const Config::ConfigPtr& configPtr) {
      if (!configPtr) {
        throw std::invalid_argument("InjectMe::configure - configPtr is a null pointer");
      }

      auto configImplPtr = dynamic_cast<ConfigImpl*>(configPtr.get());
      if (configImplPtr == nullptr) {
        throw std::invalid_argument("InjectMe::configure - internal type error");
      }

      const auto& mapTypesToProviders = configImplPtr->mapTypesToProviders;
      if (mapTypesToProviders.empty()) {
        throw std::logic_error("InjectMe::configure - no provider has been set");
      }

      // TODO
    }
  }  // namespace InjectMe
}  // namespace mf
