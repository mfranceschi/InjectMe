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
  Injected<B> b{};
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
  Injected<C> c{};
};

class C {
 public:
  C() = default;
  ~C() = default;
  NO_COPY(C);
};

void configureInjectMe() {
  auto config = Config::getInstance();
  config->add<A>()->add<B>()->add<C>();
  configure(config);
}

void runChecks() {
  auto a = inject<A>();
  auto b = inject<B>();
  myAssert(a->getB().get() == b, "Pointers have unexpected value");
  auto c = inject<C>();
  myAssert(b->getC().get() == c, "Pointers have unexpected value");
  myAssert(a->getB()->getC().get() == c, "Pointers have unexpected value");
}

int main() {
  configureInjectMe();
  runChecks();
  return 0;
}
