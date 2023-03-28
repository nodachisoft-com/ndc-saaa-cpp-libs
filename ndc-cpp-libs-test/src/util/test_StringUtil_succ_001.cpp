#include "../../pch.h"

using namespace nl;

TEST(util, isBetweenNotEq_case001)
{
  EXPECT_EQ(util::isBetweenNotEq<int>(0, 2, 1), true);
  EXPECT_EQ(util::isBetweenNotEq<int>(0, 2, 2), false);
  EXPECT_EQ(util::isBetweenNotEq<int>(0, 2, 0), false);
  EXPECT_EQ(util::isBetweenNotEq<int>(1, 2, 0), false);
}

TEST(util, isBetweenEq_case001)
{
  EXPECT_EQ(util::isBetweenEq<int>(0, 2, 1), true);
  EXPECT_EQ(util::isBetweenEq<int>(0, 2, 2), true);
  EXPECT_EQ(util::isBetweenEq<int>(0, 2, 0), true);
  EXPECT_EQ(util::isBetweenEq<int>(1, 2, 0), false);
}

// isInt のパターン確認
TEST(util, isInt_case001)
{
  EXPECT_EQ(util::isInt("+"), false);
  EXPECT_EQ(util::isInt("-"), false);
  EXPECT_EQ(util::isInt("a"), false);
  EXPECT_EQ(util::isInt("A"), false);
  EXPECT_EQ(util::isInt("."), false);
  EXPECT_EQ(util::isInt("0"), true);
  EXPECT_EQ(util::isInt("9"), true);
  EXPECT_EQ(util::isInt("+0"), true);
  EXPECT_EQ(util::isInt("-0"), true);
  EXPECT_EQ(util::isInt("12"), true);
  EXPECT_EQ(util::isInt("1."), false);
  EXPECT_EQ(util::isInt(".1"), false);
  EXPECT_EQ(util::isInt("+100"), true);
  EXPECT_EQ(util::isInt("-100"), true);
  EXPECT_EQ(util::isInt("--100"), false);
  EXPECT_EQ(util::isInt("++100"), false);
  EXPECT_EQ(util::isInt("100+1"), false);
  EXPECT_EQ(util::isInt("100-1"), false);
}

// isFloat のパターン確認
TEST(util, isFloat_case001)
{
  EXPECT_EQ(util::isFloat("+"), false);
  EXPECT_EQ(util::isFloat("-"), false);
  EXPECT_EQ(util::isFloat("a"), false);
  EXPECT_EQ(util::isFloat("A"), false);
  EXPECT_EQ(util::isFloat("."), false);
  EXPECT_EQ(util::isFloat("0"), true);
  EXPECT_EQ(util::isFloat("9"), true);
  EXPECT_EQ(util::isFloat("+0"), true);
  EXPECT_EQ(util::isFloat("-0"), true);
  EXPECT_EQ(util::isFloat("12"), true);
  EXPECT_EQ(util::isFloat("1."), true);
  EXPECT_EQ(util::isFloat(".1"), true);
  EXPECT_EQ(util::isFloat("+100"), true);
  EXPECT_EQ(util::isFloat("-100"), true);
  EXPECT_EQ(util::isFloat("--100"), false);
  EXPECT_EQ(util::isFloat("++100"), false);
  EXPECT_EQ(util::isFloat("100+1"), true);
  EXPECT_EQ(util::isFloat("100-1"), true);
  EXPECT_EQ(util::isFloat("12.34"), true);
  EXPECT_EQ(util::isFloat("12.34f"), true);
  EXPECT_EQ(util::isFloat("-12.34f"), true);
}
// isBool のパターン確認
TEST(util, isBool_case001)
{
  EXPECT_EQ(util::isBool("true"), true);
  EXPECT_EQ(util::isBool("TrUE"), true);
  EXPECT_EQ(util::isBool("TRUE"), true);
  EXPECT_EQ(util::isBool("ABCD"), false);
  EXPECT_EQ(util::isBool("FalSE"), true);
  EXPECT_EQ(util::isBool("false"), true);
}
