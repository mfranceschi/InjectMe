#include "Database.hpp"
#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace internals
    {
      void reset() {
        Database::getInstance().reset(true);
      }
    }  // namespace internals
  }    // namespace InjectMe
}  // namespace mf
