#include "../../pch.h"

using namespace nl;

// 新しく画像データを作成し、左上に赤い横線を引く
TEST(BitmapImage, case1)
{
  std::string filepath(TESTTMP_DIR + "test_BitmapImage_case1.bmp");

  // 事前準備（テストファイル削除）
  FileAccessMgr::removefile(filepath);
  ImageCanvas canvas(16, 16);
  BitmapImage image;
  for (int i = 0; i < 5; i++)
  {
    ColorRGB COLOR_RED{0xff, 0x00, 0x00};
    canvas.set(i, 2, COLOR_RED);
  }
  image.WriteBmp(filepath, canvas);

  // TODO: 正解ファイルとの HASH 値比較
}

// 画像データを読み込み、別ファイルに出力する
TEST(BitmapImage, case2)
{
  // 事前準備（テストファイル削除）
  {
    std::string filepathPre(TESTTMP_DIR + "test_BitmapImage_case2_preRead.bmp");
    FileAccessMgr::removefile(filepathPre);
    ImageCanvas canvasPre(16, 16);
    BitmapImage imagePre;
    for (int i = 0; i < 5; i++)
    {
      ColorRGB COLOR_RED{0xff, 0x00, 0x00};
      canvasPre.set(i, 2, COLOR_RED);
    }
    imagePre.WriteBmp(filepathPre, canvasPre);
  }

  // テスト実施
  {
    std::string filepathRead(TESTTMP_DIR + "test_BitmapImage_case2_preRead.bmp");
    std::string filepathWrite(TESTTMP_DIR + "test_BitmapImage_case2_aftWrite.bmp");

    // 事前準備（テストファイル削除）
    FileAccessMgr::removefile(filepathWrite);
    BitmapImage image;
    std::unique_ptr<ImageCanvas> canvas = image.ReadBmp(filepathRead);
    image.WriteBmp(filepathWrite, *canvas.get());
  }
}

// 画像データのヘッダー情報を確認する
/*
TEST(BitmapImage, check_headerbytes_succ001)
{
  std::string filepath(TESTTMP_DIR + "test_BitmapImage_case2.bmp");

  // 事前準備（テストファイル削除）
  FileAccessMgr::removefile(filepath);
  BitmapImage image("..\\..\\ndc-cpp-libs-test\\src\\image\\test_bmp_001.bmp");
  image.WriteBmp(filepath);
}
*/