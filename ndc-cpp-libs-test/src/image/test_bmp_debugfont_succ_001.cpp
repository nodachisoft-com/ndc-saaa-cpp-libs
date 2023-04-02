#include "../../pch.h"


using namespace nl;

// 新しく画像データを作成して、１文字を描画する
TEST(BitmapImage, writeChar_case001)
{
  BitmapImage image(50, 50);
  ImageCanvas *canvas = image.getRefImageCanvas();
  
  ColorRGB BASE_COLOR{0x00, 0xa0, 0x10}; // 緑っぽい
  canvas->clear(BASE_COLOR);

  ColorRGB COLOR_LIST[] = {
      {0xff, 0x00, 0x00}, // 赤
      {0x00, 0xff, 0x00}, // 緑
      {0x00, 0x00, 0xff}, // 青
  };

  int i = 0;
  for (int v = -20; v < 71; v += 15)
  {
    for (int u = -20; u < 71; u += 15)
    {
      canvas->writeChar(u, v, (char)('A' + (u * u + v * v) % 20), COLOR_LIST[i % 3]);
      i++;
    }
  }

  const std::string path(TESTTMP_DIR + "test_writeChar_case001.bmp");
  image.WriteBmp(path);
  // 改めてファイルを読み込み CRC32 の検査を行う
  FileAccessor faRead(path);
  faRead.readFileSync();
  // EXPECT_EQ(848973220, faRead.calcMemoryCrc32());
  EXPECT_EQ(2711833527, faRead.calcMemoryCrc32());

}

// 新しく画像データを作成して、文字列を描画する
TEST(BitmapImage, writeText_case001)
{
  BitmapImage image(300, 50);
  ImageCanvas* canvas = image.getRefImageCanvas();
  ColorRGB BASE_COLOR{0x00, 0x50, 0x10};
  canvas->clear(BASE_COLOR);

  ColorRGB COLOR_LIST[] = {
      {0xff, 0x00, 0x00}, // 赤
      {0x00, 0xff, 0x00}, // 緑
      {0x00, 0x00, 0xff}, // 青
  };

  int i = 0;
  for (int v = -10; v < 21; v += 15)
  {
    canvas->writeText(-20, v, "This is a test message!12345678901234567890", COLOR_LIST[i % 3]);
    i++;
  }

  const std::string path(TESTTMP_DIR + "test_writeText_case001.bmp");
  image.WriteBmp(path);
  // 改めてファイルを読み込み CRC32 の検査を行う
  FileAccessor faRead(path);
  faRead.readFileSync();
  // EXPECT_EQ(1611399988, faRead.calcMemoryCrc32());
  EXPECT_EQ(486548973, faRead.calcMemoryCrc32());
}
