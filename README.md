# InjectMe

[![My_CI](https://github.com/mfranceschi/InjectMe/actions/workflows/My_CI.yaml/badge.svg)](https://github.com/mfranceschi/InjectMe/actions/workflows/My_CI.yaml)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
![C++14](https://img.shields.io/badge/Made%20for-C%2B%2B14-blue)

C++ dependency injector.

! Work in progress !

## How to use?

### CMake configuration

(TODO) CMake FetchContent with Git repo on some tag + add_subdirectory + link MF_InjectMe library to your program

### Use in code: header and namespace.

After the previous steps, the whole library can be accessed by simply including the following header:

```cpp
#include "InjectMe.hpp"
```

All the code (classes, functions, etc.) is scoped in the namespace `mf::InjectMe::`.

> :information_source: Some functions are available in the namespace `mf::InjectMe::advanced::` but they are for advanced users only - they exist mostly for testing.

### Use in code: configuration

First, your `main` function should register as quickly as possible the different types to inject. For each type, you have to pass it a function which returns a heap-allocated pointer to the intended value. Quick example:

```cpp
void configureInjectMe() {
  using mf::InjectMe::Config;
  using mf::InjectMe::configure;

  auto config = Config::getInstance();
  config
      ->add<A>([]() {
        return new A;
      })
      ->add<B>([]() {
        return new B;
      })
      ->add<C>([]() {
        return new C;
      });
  configure(config);
}

int main() {
  configureInjectMe();
  // ...
  return 0;
}
```

You can call the `configure(...)` function several times in your program, as long as it does not contain a provider for an already-known type.

### Use in code: inject!

When you want to inject some value, use `inject<type>()`; it will return an instance of `Injected<type>` which is to be used like a pointer. Example:

```cpp
class A {
  auto myInt = inject<int>(); // I would recommend using "auto" for the type.
};

// ...

void f() {
  Injected<A> myA = inject<A>();
}
```

### Things to be aware of

- `using`s and `typedef`s: if you have types `A` and `B` where `B` is defined with `using B = A;`, trying to register both will cause a duplication error.
- Instances of your types are generated lazily = only at the first `inject` call for each type.
- All instances are cleanly deleted at program termination.

## Technical infos

Recommended setup: C++14, CMake 3.16. 

Supported platforms and compilers:
- Ubuntu: clang, g++
- Windows: MSVC
