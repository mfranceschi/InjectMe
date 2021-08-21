#include "Database.hpp"
#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace advanced
    {
      void reset() {
        Database::getInstance().reset(true);
      }
    }  // namespace advanced
  }    // namespace InjectMe
}  // namespace mf
