#include <gtest/gtest.h>

int main(int nbrArgs, char **argsList) {
  ::testing::InitGoogleTest(&nbrArgs, argsList);
  return RUN_ALL_TESTS();
}
