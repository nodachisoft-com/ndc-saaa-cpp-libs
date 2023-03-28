#include "../../pch.h"

using namespace nl;

// 初期化が想定通りであること
TEST(Vector2d, const_succ001)
{

  Vector2d v1;

  EXPECT_EQ(v1.x, 0.0f);
  EXPECT_EQ(v1.y, 0.0f);

  Vector2d v2(5.0f, 8.0f);
  EXPECT_EQ(v2.x, 5.0f);
  EXPECT_EQ(v2.y, 8.0f);
  EXPECT_EQ(v2.magnitude(), (float)std::sqrt(5 * 5 + 8 * 8));
}

// Vector 同士や Vector とスカラー値の計算が想定通りであること
TEST(Vector2d, operation_succ001)
{

  Vector2d v1(1.0f, 2.0f);
  Vector2d v2(3.0f, 4.0f);

  // ベクトル同士の足し算
  Vector2d test1 = v1 + v2;
  EXPECT_EQ(test1.x, 1.0f + 3.0f);
  EXPECT_EQ(test1.y, 2.0f + 4.0f);

  // ベクトル同士の引き算
  Vector2d test2 = v1 - v2;
  EXPECT_EQ(test2.x, 1.0f - 3.0f);
  EXPECT_EQ(test2.y, 2.0f - 4.0f);

  // ベクトルとスカラー値の掛け算
  Vector2d test3 = v1 * 2;
  EXPECT_EQ(test3.x, 1.0f * 2);
  EXPECT_EQ(test3.y, 2.0f * 2);

  // ベクトルとスカラー値の割り算
  Vector2d test4 = v1 / 2.0f;
  EXPECT_EQ(test4.x, 1.0f / 2.0f);
  EXPECT_EQ(test4.y, 2.0f / 2.0f);
}
