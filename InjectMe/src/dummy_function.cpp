#include <limits>
#include <stdexcept>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    int addIntegersOrThrow(int numberA, int numberB) {
      // adapted from
      // https://stackoverflow.com/questions/55468823/how-to-detect-integer-overflow-in-c
      if ((numberB > 0) && (numberA > std::numeric_limits<int>::max() - numberB)) {
        throw std::overflow_error("a+b will overflow");
      }

      if ((numberB < 0) && (numberA < std::numeric_limits<int>::min() - numberB)) {
        throw std::underflow_error("a+b will underflow");
      }

      return numberA + numberB;
    }
  }  // namespace InjectMe
}  // namespace mf
