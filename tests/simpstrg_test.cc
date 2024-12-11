#include "simpstrg.h"
#include "gtest/gtest.h"

TEST(Initialization, DefaultConstructor) {
  simpstrg s;
  ASSERT_EQ(s, "");
}

TEST(Initialization, CharConstructor) {
  simpstrg s("abc");  // simpstrg s == "abc" not allowed.
  ASSERT_EQ(s, "abc");
}

TEST(Initialization, StringConstructor) {
  simpstrg s("abc");
  simpstrg t = s;
  ASSERT_EQ(s, t);
  ASSERT_EQ(t, "abc");
}

TEST(Initialization, Concatenation) {
  simpstrg s("abc");
  simpstrg t("def");
  simpstrg v = s + t;
  ASSERT_EQ(v, "abcdef");
  v += "ghi";
  ASSERT_EQ(v, "abcdefghi");
}

TEST(Initialization, Substring) {
  simpstrg s("abcdefghij");
  ASSERT_EQ(s(4, 3), "efg");
  ASSERT_EQ(s(20, 3), "");
  ASSERT_EQ(s(-5, 4), "fghi");
  ASSERT_EQ(s(4, -2), "efghi");
  ASSERT_EQ(s(5, 100), "fghij");
}
