#include "InjectMe.hpp"
#include "limits"
#include "stdexcept"

namespace mf
{
    namespace InjectMe
    {
        int addIntegersOrThrow(int a, int b)
        {
            // adapted from https://stackoverflow.com/questions/55468823/how-to-detect-integer-overflow-in-c
            if ((b > 0) && (a > std::numeric_limits<int>::max() - b))
            {
                throw std::overflow_error("a+b will overflow");
            }
            else if ((b < 0) && (a < std::numeric_limits<int>::min() - b))
            {
                throw std::underflow_error("a+b will underflow");
            }
            return a + b;
        }
    } // namespace InjectMe
} // namespace mf
