#include "../../pch.h"

using namespace nl;

// 10:20:30 の割合でオブジェクトを登録し、1000 回の試行で想定に近い回数で
// それぞれのオブジェクトが取得できることを確認する
TEST(WeightRandom, succ_3objects_case1)
{
  WeightRandom<std::string> wr;
  wr.init(100);
  wr.push(10, "Apple");
  wr.push(20, "Orange");
  wr.push(30, "Banana");

  // 合計の確率
  float sumRandomPerc = wr.getSumRandomPerc();
  EXPECT_EQ(sumRandomPerc, 60);

  int tryCount = 1000;
  float acceptableDelta = 0.1f; // 10 % 程度の確率分布の誤差を許容

  int appleCnt = 0;
  int orangeCnt = 0;
  int bananaCnt = 0;
  for (int i = 0; i < tryCount; i++)
  {
    std::string str = wr.getRandElem();
    if (str == "Apple")
    {
      appleCnt++;
    }
    else if (str == "Orange")
    {
      orangeCnt++;
    }
    else if (str == "Banana")
    {
      bananaCnt++;
    }
  }
  // DEBUG 時にコメントアウト
  // std::cout << "Apple Count: " << appleCnt << std::endl;
  // std::cout << "Orange Count: " << orangeCnt << std::endl;
  // std::cout << "Banana Count: " << bananaCnt << std::endl;

  // Apple の出現確率が 想定と 10 % 以内の誤差であること
  float applePerc = (appleCnt / (float)tryCount);
  EXPECT_GT(acceptableDelta, std::abs(applePerc - (10.0f / sumRandomPerc)));

  // Orange の出現確率が 想定と 10 % 以内の誤差であること
  float orangePerc = (orangeCnt / (float)tryCount);
  EXPECT_GT(acceptableDelta, std::abs(orangePerc - (20.0f / sumRandomPerc)));

  // Banana の出現確率が 想定と 10 % 以内の誤差であること
  float bananaPerc = (bananaCnt / (float)tryCount);
  EXPECT_GT(acceptableDelta, std::abs(bananaPerc - (30.0f / sumRandomPerc)));
};

// 1つのオブジェクトを登録し、1000 回の試行で想定に近い回数で
// 1つのオブジェクトが取得できることを確認する
TEST(WeightRandom, succ_1objects_case1)
{
  WeightRandom<std::string> wr;
  wr.init(100);
  wr.push(10000, "Apple");

  // 合計の確率
  float sumRandomPerc = wr.getSumRandomPerc();
  EXPECT_EQ(sumRandomPerc, 10000);

  int tryCount = 1000;
  float acceptableDelta = 0.1f; // 10 % 程度の確率分布の誤差を許容

  int appleCnt = 0;
  for (int i = 0; i < tryCount; i++)
  {
    std::string str = wr.getRandElem();
    if (str == "Apple")
    {
      appleCnt++;
    }
  }
  EXPECT_EQ(appleCnt, tryCount);
};

// 100 個のランダムな重みをもつオブジェクトを登録し、1000 * 1000 回の試行で想定に近い回数で
// それぞれのオブジェクトが取得できることを確認する
TEST(WeightRandom, succ_100objects_case1)
{
  MtRandomizer rand(100);
  std::vector<float> weightList;
  float weightSum = 0.0f;
  WeightRandom<int> wr;
  int resultCount[100] = {0};

  wr.init(100);
  for (int i = 0; i < 100; i++)
  {
    float weight = rand.getRndFloat(0, 100.0f);
    weightSum += weight;
    weightList.push_back(weight);
    wr.push(weight, i);
  }

  // 合計の確率
  float sumRandomPerc = wr.getSumRandomPerc();
  EXPECT_EQ(sumRandomPerc, weightSum);

  long tryCount = 1000 * 1000;
  float acceptableDelta = 0.1f; // 10 % 程度の確率分布の誤差を許容

  for (long i = 0; i < tryCount; i++)
  {
    int res = wr.getRandElem();
    resultCount[res]++;
  }

  for (int i = 0; i < 100; i++)
  {
    float perc = (resultCount[i] / (float)tryCount);
    EXPECT_GT(acceptableDelta, std::abs(perc - (weightList[i] / sumRandomPerc)));
  }
};
