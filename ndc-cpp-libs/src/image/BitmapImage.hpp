/**
 * @brief デバッグ等に利用可能な Bitmap 画像形式の読み込み、書き込み機能を提供する
 */
#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
// #include <stdlib.h>
#include <cstdlib>

#include "ImageCanvas.hpp"
#include "BitmapMetainfo.hpp"
#include "ColorRGB.hpp"



namespace nl
{


  /// @brief Bitmap １つの画像ファイル、データを扱う
  class BitmapImage
  {
  private:

    /// 画像データ本体
    ImageCanvas* imgp = nullptr;

    // Metainfo
    BitmapMetainfo metainfo;

  public:
    /// @brief コンストラクタ
    BitmapImage(const std::string filename);
    BitmapImage(const char *filename);
    BitmapImage(const int width, const int height);

    /// デストラクタ
    ~BitmapImage();

    // 操作
    void ReadBmp(const std::string filename);
    void ReadBmp(const char *filename);
    void WriteBmp(const std::string filename);
    void WriteBmp(const char *filename);
    void PrintBmpInfo(const std::string filename);
    void PrintBmpInfo(const char *filename);

    /// <summary>
    /// BitmapImage に保持されている ImageCanvas への参照を取得する
    /// </summary>
    /// <returns>ImageCanvas への参照</returns>
    ImageCanvas* getRefImageCanvas()
    {
      return imgp;
    }

  };
};
