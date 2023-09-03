#include "../IntegrationTestHelpers.hpp"
#include "InjectMe.hpp"

using namespace mf::InjectMe;

// Forward declaration
class B;

class A {
 public:
  A() = default;
  NO_COPY(A);
  ~A() = default;

 private:
  Injected<int> theInt;
  Injected<B> theB;
};

class B {
 public:
  B() = default;
  NO_COPY(B);
  ~B() = default;

 private:
  Injected<A> theA;
};

class C {
 public:
  C() = default;
  NO_COPY(C);
  ~C() = default;

 private:
  Injected<C> theC;
};

void configureInjector() {
  configure<A>().done();
  configure<B>().done();
  configure<C>().done();
  configure<int>()
      .setProvider([]() {
        return new int(42);
      })
      .done();
}

void checkRecursionOfAIsCaught() {
  try {
    inject<A>();
    myAssert(false, "an 'A' injected pointer has been provided!");
  } catch (const exceptions::ProviderRecursion&) {
    // It's okay.
  }
}

void checkRecursionOfCIsCaught() {
  try {
    inject<C>();
    myAssert(false, "a 'C' injected pointer has been provided!");
  } catch (const exceptions::ProviderRecursion&) {
    // It's okay.
  }
}

int main() {
  configureInjector();

  checkRecursionOfAIsCaught();
  checkRecursionOfCIsCaught();
  return 0;
}
