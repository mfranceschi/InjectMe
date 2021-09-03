#pragma once

#include "InjectMe_configuration.hpp"
#include "InjectMe_exceptions.hpp"
#include "InjectMe_inject.hpp"
#include "InjectMe_types.hpp"

namespace mf
{
  namespace InjectMe
  {
    /**
     * Dummy function for testing purpose.
     * @return the sum of a and b.
     * @throws std::overflow_error or std::underflow_error.
     */
    int addIntegersOrThrow(int a, int b);
  }  // namespace InjectMe
}  // namespace mf
