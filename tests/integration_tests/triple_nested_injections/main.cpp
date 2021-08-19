#include "../IntegrationTestHelpers.hpp"
#include "InjectMe.hpp"

using namespace mf::InjectMe;

class A;
class B;
class C;

class A {
 public:
  A() = default;
  ~A() = default;
  NO_COPY(A);
  Injected<B> getB() {
    return b;
  }

 private:
  Injected<B> b = inject<B>();
};

class B {
 public:
  B() = default;
  ~B() = default;
  NO_COPY(B);
  Injected<C> getC() {
    return c;
  }

 private:
  Injected<C> c = inject<C>();
};

class C {
 public:
  C() = default;
  ~C() = default;
  NO_COPY(C);
};

void configureInjectMe() {
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

void runChecks() {
  auto a = inject<A>();
  auto b = inject<B>();
  myAssert(a->getB() == b, "Pointers have unexpected value");
  auto c = inject<C>();
  myAssert(b->getC() == c, "Pointers have unexpected value");
  myAssert(a->getB()->getC() == c, "Pointers have unexpected value");
}

int main() {
  configureInjectMe();
  runChecks();
  return 0;
}
