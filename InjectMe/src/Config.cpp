#include "ConfigImpl.hpp"

namespace mf
{
  namespace InjectMe
  {
    std::unique_ptr<Config> Config::getInstance() {
      return std::make_unique<ConfigImpl>();
    }
  }  // namespace InjectMe

}  // namespace mf
