#include "../../pch.h"

using namespace nl;

// 初期化が想定通りであること
TEST(Rectangle2d, const_succ001)
{

  Rectangle2d r1;

  EXPECT_EQ(r1.getLeftX(), 0.0f);
  EXPECT_EQ(r1.getRightX(), 0.0f);
  EXPECT_EQ(r1.getUpperY(), 0.0f);
  EXPECT_EQ(r1.getBottomY(), 0.0f);
  EXPECT_EQ(r1.getWidth(), 0.0f);
  EXPECT_EQ(r1.getHeight(), 0.0f);
  EXPECT_EQ(r1.getLeftUpperPoint().x, 0.0f);
  EXPECT_EQ(r1.getLeftUpperPoint().y, 0.0f);
  EXPECT_EQ(r1.getRightBottomPoint().x, 0.0f);
  EXPECT_EQ(r1.getRightBottomPoint().y, 0.0f);

  // p1:左下, p2:右上 で初期化
  Rectangle2d r2({-5, -4}, {-3, -2});

  EXPECT_EQ(r2.getLeftX(), -5.0f);
  EXPECT_EQ(r2.getRightX(), -3.0f);
  EXPECT_EQ(r2.getUpperY(), -4.0f);
  EXPECT_EQ(r2.getBottomY(), -2.0f);
  EXPECT_EQ(r2.getWidth(), 2.0f);
  EXPECT_EQ(r2.getHeight(), 2.0f);
  EXPECT_EQ(r2.getLeftUpperPoint().x, -5.0f);
  EXPECT_EQ(r2.getLeftUpperPoint().y, -4.0f);
  EXPECT_EQ(r2.getRightBottomPoint().x, -3.0f);
  EXPECT_EQ(r2.getRightBottomPoint().y, -2.0f);
}
