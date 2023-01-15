#include "includes.h"
#include "grid.h"
#include "gtest/gtest.h"

TEST(Initialization, DefaultConstructor) {
  grid<int> g;
  ASSERT_EQ(g.rows(), 0);
  ASSERT_EQ(g.cols(), 0);
}

