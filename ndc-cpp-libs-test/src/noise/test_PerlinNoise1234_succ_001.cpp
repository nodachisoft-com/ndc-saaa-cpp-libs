#include "../../pch.h"

#include <math.h>


using namespace nl;

// PerlinNoise で同じ位置で同じ結果が得られるかの確認を行う
TEST(PerlinNoise1234, MT19937_HF_case001)
{
  std::ostringstream oss;
  PerlinNoise1234 pn(100);
  float sum = 0.0f;
  bool equalFlag = true;
  for (int u = 0; u < 256; u++)
  {
    float memory = pn.pnoise2(u / 6.0f, u * 1.03f / 4.0f, 6, 6, 255.0f);
    for (int tryCount = 0; tryCount < 3; tryCount++)
    {
      if (memory != pn.pnoise2(u / 6.0f, u * 1.03f / 4.0f, 6, 6, 255.0f))
      {
        equalFlag = false;
      }
    }
    sum += memory;
  }
  oss << std::fixed << std::setprecision(5) << sum;
  EXPECT_EQ(oss.str(), "32438.62891");
  EXPECT_EQ(equalFlag, true);
}

// PPN で再現可能な HeightField 画像を生成し、CRC32 でチェックする
TEST(PerlinNoise1234, MT19937_HF_case002)
{
  int width = 70;
  int height = 46;
  ImageCanvas canvas(width, height);
  BitmapImage image;
  PerlinNoise1234 pn(100);
  int noisePointX = 8;
  int noisePointY = 8;

  Crc32 crc;
  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      float res = pn.pnoise2((u * noisePointX) / (float)width, (v * noisePointY) / (float)height, noisePointX, noisePointY, 255.0f);
      unsigned char color_elem = (unsigned char)res;
      crc.calcUpdate(color_elem);
      ColorRGB color{color_elem, color_elem, color_elem};
      canvas.set(u, v, color);
    }
  }
  image.WriteBmp(TESTTMP_DIR + "MT19937_HF_case002.bmp", canvas);

  // CRC32 を取得する
  FileAccessor fa(TESTTMP_DIR + "MT19937_HF_case002.bmp");
  EXPECT_EQ(fa.readFileSync(), true);
  EXPECT_EQ(fa.getFilesize(), 9806);
  EXPECT_EQ(fa.calcMemoryCrc32(), 1842797783); // BITMAP 構造体 + データ本体
  EXPECT_EQ(crc.getHash(), 2085138398);       // データ本体のみの CRC32
}

// デフォルトコンストラクタ後に改めて初期化を行うケースで正常に処理が行えること
// 2回目以降で 初期化は失敗すること
TEST(PerlinNoise1234, Constructor_case001)
{
  PerlinNoise1234 pn;
  EXPECT_EQ(pn.init(100), true);  // 初期化に成功
  EXPECT_EQ(pn.init(200), false); // 初期化に失敗
  int width = 70;
  int height = 46;
  int noisePointX = 8;
  int noisePointY = 8;

  Crc32 crc;
  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      float res = pn.pnoise2((u * noisePointX) / (float)width, (v * noisePointY) / (float)height, noisePointX, noisePointY, 255.0f);
      crc.calcUpdate((unsigned char)res);
    }
  }
  EXPECT_EQ(crc.getHash(), 2085138398); // データ本体のみの CRC32
}

// コンストラクタで初期化しているなら２回目の初期化に失敗すること
TEST(PerlinNoise1234, Constructor_case002)
{
  PerlinNoise1234 pn(100);
  EXPECT_EQ(pn.init(200), false);
  int width = 70;
  int height = 46;
  int noisePointX = 8;
  int noisePointY = 8;

  Crc32 crc;
  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      float res = pn.pnoise2((u * noisePointX) / (float)width, (v * noisePointY) / (float)height, noisePointX, noisePointY, 255.0f);
      crc.calcUpdate((unsigned char)res);
    }
  }
  EXPECT_EQ(crc.getHash(), 2085138398); // データ本体のみの CRC32
}
