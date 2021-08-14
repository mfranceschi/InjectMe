#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    void configure(const Config::ConfigPtr& configPtr) {
      if (configPtr) {  // TODO
      } else {
        throw std::invalid_argument("");
      }
    }
  }  // namespace InjectMe
}  // namespace mf
