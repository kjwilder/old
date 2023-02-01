#include "timer.h"
#include "gtest/gtest.h"

TEST(Initialization, ATest) {
  timer t;
  ASSERT_EQ(t.check(), 0);
  t.start();
  double time1 = t.check();
  ASSERT_GT(time1, 0);
  t.stop();
  double time2 = t.check();
  ASSERT_GE(time2, time1);
  double time3 = t.check();
  ASSERT_EQ(time3, time2);
}
