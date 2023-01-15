#include "includes.h"
#include "avlgrid.h"
#include "gtest/gtest.h"

TEST(Initialization, DefaultConstructor) {
  avlgrid<int> g(100, 2);
  vector<int> v(2);
  v[0] = 9; v[1] = 3; g.insert(v);
  v[0] = 1; v[1] = 4; g.insert(v);
  v[0] = 2; v[1] = 7; g.insert(v);
  v[0] = 1; v[1] = 8; g.insert(v);
  v[0] = 1; v[1] = 2; g.insert(v);
  testing::internal::CaptureStdout();
  g.dump();
  ASSERT_EQ(g.rows(), 100);
  ASSERT_EQ(testing::internal::GetCapturedStdout(),
      "1 2 \n1 4 \n1 8 \n2 7 \n9 3 \n");
}

