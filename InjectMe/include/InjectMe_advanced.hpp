#pragma once

namespace mf
{
  namespace InjectMe
  {
    namespace advanced
    {
      /**
       * Calls all Provider functions so that all instances are ready for all types.
       */
      void callAllProviders();

      /**
       * WARNING: ADVANCED USERS ONLY.
       * Forgets all types and providers configured and deletes all objects.
       * All Injected<T> pointers are invalidated (they point to freed memory).
       */
      void reset();
    }  // namespace advanced
  }  // namespace InjectMe
}  // namespace mf
