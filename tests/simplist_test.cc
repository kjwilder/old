#include "simplist.h"
#include "gtest/gtest.h"

TEST(Initialization, ATest) {
  simplist<int> l;
  l.insert(5);
  l.insert(23);
  l.insert(4);
  ASSERT_EQ(3, l.length());
}
