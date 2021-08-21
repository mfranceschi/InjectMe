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
  Injected<int> theInt = inject<int>();
  Injected<B> theB = inject<B>();
};

class B {
 public:
  B() = default;
  NO_COPY(B);
  ~B() = default;

 private:
  Injected<A> theA = inject<A>();
};

class C {
 public:
  C() = default;
  NO_COPY(C);
  ~C() = default;

 private:
  Injected<C> theC = inject<C>();
};

void configureInjector() {
  auto configPtr = Config::getInstance();
  configPtr
      ->add<A>([]() {
        return new A();
      })
      ->add<B>([]() {
        return new B();
      })
      ->add<C>([]() {
        return new C();
      })
      ->add<int>([]() {
        return new int(42);
      });
  configure(configPtr);
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
