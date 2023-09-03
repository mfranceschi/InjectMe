#pragma once

#include <typeinfo>

#include "InjectMe_types.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace internals
    {
      void* injectForTypeOrThrow(const std::type_info&);
    }  // namespace internals

    /**
     * @returns a pointer to an instance of the given type (create if never queried before).
     * @throws std::logic_error if no Provider is known for that type.
     */
    template <typename T>
    T* inject() {
      return static_cast<T*>(internals::injectForTypeOrThrow(typeid(T)));
    }

    /**
     * Smart pointer-like! Cannot contain a null pointer.
     */
    template <typename T>
    class Injected {
     public:
      Injected<T>() = default;
      explicit Injected<T>(T* pointer) : pointer(pointer) {
      }
      Injected(const Injected<T>&) = default;
      Injected(Injected<T>&&) noexcept = default;
      Injected<T>& operator=(const Injected<T>&) = default;
      Injected<T>& operator=(Injected<T>&&) noexcept = default;
      ~Injected() = default;

      T* get() const {
        return pointer;
      }
      T& operator*() const {
        return *pointer;
      }
      T* operator->() const {
        return pointer;
      }
      operator T*() {
        return pointer;
      }

     private:
      T* pointer = inject<T>();
    };
  }  // namespace InjectMe
}  // namespace mf
