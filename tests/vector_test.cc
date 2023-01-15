#include "includes.h"
#include "vector.h"
#include "gtest/gtest.h"

TEST(Initialization, DefaultConstructor) {
  vector<int> v;
  ASSERT_EQ(v.dim(), 0);
}

TEST(Initialization, SizeConstructor) {
  vector<int> v(4);
  v[0] = 3; v[1] = 2; v[2] = 5; v[3] = 8;
  ASSERT_EQ(v.dim(), 4);
}

TEST(Initialization, Addition) {
  vector<int> v(2), w(2);
  v[0] = 2; v[1] = 8;
  w[0] = -4; w[1] = 4;
  v += w;
  ASSERT_EQ(v.dim(), 2);
  ASSERT_EQ(v[0], -2);
  ASSERT_EQ(v[1], 12);
  ASSERT_EQ(v(0), -2);
  ASSERT_EQ(v(1), 12);
}

TEST(Initialization, Clear) {
  vector<int> v(2);
  v[0] = 2; v[1] = 8;
  v.clear();
  ASSERT_EQ(v[0], 0);
  ASSERT_EQ(v[1], 0);
  v.clear(5);
  ASSERT_EQ(v[0], 5);
  ASSERT_EQ(v[1], 5);
}

