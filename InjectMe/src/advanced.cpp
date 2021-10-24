#include "Database.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace advanced
    {
      void callAllProviders() {
        Database::getDatabase().provideForAll();
      }

      void reset() {
        Database::getDatabase().reset();
      }
    }  // namespace advanced
  }  // namespace InjectMe
}  // namespace mf
