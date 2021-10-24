#pragma once

#include <functional>

namespace mf
{
  namespace InjectMe
  {
    template <typename T>
    using ProviderFct = std::function<T*(void)>;

    template <typename T>
    using DeleterFct = std::function<void(T*)>;
  }  // namespace InjectMe
}  // namespace mf
