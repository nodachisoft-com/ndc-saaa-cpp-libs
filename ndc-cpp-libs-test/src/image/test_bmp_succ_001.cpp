﻿#include "../../pch.h"

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
  std::string filepath(TESTTMP_DIR + "test_BitmapImage_case2.bmp");
  // 事前準備（テストファイル削除）
  FileAccessMgr::removefile(filepath);
  BitmapImage image;
  std::unique_ptr<ImageCanvas> canvas =   
    image.ReadBmp("..\\..\\ndc-cpp-libs-test\\src\\image\\test_bmp_001.bmp");

  image.WriteBmp(filepath, *canvas.get());
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