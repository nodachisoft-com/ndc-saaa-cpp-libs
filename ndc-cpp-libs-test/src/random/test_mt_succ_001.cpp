#include "../../pch.h"

using namespace nl;

// 整数で[0,10]の範囲で 10,000 回乱数取得し、
// 生成された値の偏りが 1 割以上の差となっていないことを
// 確認する
TEST(random, case1)
{

  MtRandomizer rand(100);

  int c[10] = {};
  for (int i = 0; i < 10000; i++)
  {
    int randnum = rand.getRndInt(0, 9);
    c[randnum]++;
  }
  for (int i = 0; i < 10; i++)
  {
    int d = c[i];
    int lookfor = 10000 / 10;
    if (d <= 0.9f * lookfor || lookfor * 1.1f <= d)
    {
      // 確率ばらけすぎでは
      ASSERT_TRUE(false);
    }
  }
};

// [5,8] の範囲で正しく取得可能なこと
TEST(random, case2)
{
  MtRandomizer rand(100);
  int min = INT32_MAX;
  int max = INT32_MIN;
  for (int i = 0; i < 1000; i++)
  {
    int rnd = rand.getRndInt(5, 8);
    if (rnd < min)
    {
      min = rnd;
    }
    if (rnd > max)
    {
      max = rnd;
    }
  }
  EXPECT_EQ(min, 5);
  EXPECT_EQ(max, 8);
};

// float を 10 個 x 3 回生成して、毎回、同じ結果の想定通りの結果であること
TEST(random, MT19937_getRndFloat_case001)
{
  // 乱数生成の結果想定
  const std::string expectStr = " 67.116 41.205 52.638 14.860 15.671 18.647 21.011 45.274 87.014 6.368";
  for (int cnt = 0; cnt < 3; cnt++)
  {
    MtRandomizer rand(100);
    std::ostringstream oss;
    for (int i = 0; i < 10; i++)
    {
      float val = rand.getRndFloat(0.0, 100.0f);
      oss << std::fixed << std::setprecision(3) << " " << val;
    }
    EXPECT_EQ(oss.str(), expectStr);
  }
}

// 乱数シードを変化させた状態で float を 5 個 x 3 回生成して
// 毎回、想定通りの異なる結果を得られること
TEST(random, MT19937_getRndFloat_case002)
{
  // 乱数生成の結果想定
  const std::string expectStr[3] = {
      " 67.11556 41.20464 52.63824 14.86048 15.67111",
      " 55.25402 95.26843 98.44476 0.13304 53.41109",
      " 39.75574 46.26012 31.02066 51.68619 17.23068"};

  for (int cnt = 0; cnt < 3; cnt++)
  {
    MtRandomizer rand(100 + cnt);
    std::ostringstream oss;
    for (int i = 0; i < 5; i++)
    {
      float val = rand.getRndFloat(0.0, 100.0f);
      oss << std::fixed << std::setprecision(5) << " " << val;
    }
    EXPECT_EQ(oss.str(), expectStr[cnt]);
  }
}

// 乱数シードを変化させた状態で float を 5 個 x 3 回生成して
// 毎回、想定通りの異なる結果を得られること。生成範囲はマイナスを含む
TEST(random, MT19937_getRndFloat_case003)
{
  // 乱数生成の結果想定
  const std::string expectStr[3] = {
      " 3.423 -1.759 0.528 -7.028 -6.866",
      " 1.051 9.054 9.689 -9.973 0.682",
      " -2.049 -0.748 -3.796 0.337 -6.554"};

  for (int cnt = 0; cnt < 3; cnt++)
  {
    MtRandomizer rand(100 + cnt);
    std::ostringstream oss;
    for (int i = 0; i < 5; i++)
    {
      float val = rand.getRndFloat(-10.0, 10.0f);
      oss << std::fixed << std::setprecision(3) << " " << val;
    }
    EXPECT_EQ(oss.str(), expectStr[cnt]);
  }
}

// 乱数シードを変化させた状態で int を 10 個 x 3 回生成して
// 毎回、想定通りの異なる結果を得られること
TEST(random, MT19937_getRndInt_case001)
{
  // 乱数生成の結果想定
  const std::string expectStr[3] = {
      "1 4 -5 -2 -2 ",
      "0 1 1 10 -10 ",
      "2 -2 4 -1 -4 "};

  for (int cnt = 0; cnt < 3; cnt++)
  {
    MtRandomizer rand(100 + cnt);
    std::ostringstream oss;
    for (int i = 0; i < 5; i++)
    {
      int val = rand.getRndInt(-10, 10);
      oss << val << " ";
    }
    EXPECT_EQ(oss.str(), expectStr[cnt]);
  }
}
