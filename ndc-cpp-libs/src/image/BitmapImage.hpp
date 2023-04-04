/**
 * @brief デバッグ等に利用可能な Bitmap 画像形式の読み込み、書き込み機能を提供する
 */
#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include "../log/Logger.hpp"
#include "ImageCanvas.hpp"
#include "BitmapMetainfo.hpp"
#include "ColorRGB.hpp"
#include "../exception/index.hpp"



namespace nl
{


  /// @brief Bitmap １つの画像ファイル、データを扱う
  class BitmapImage
  {
  private:

    // Metainfo
    BitmapMetainfo metainfo;

  public:
    /// @brief コンストラクタ
    BitmapImage();


    /// デストラクタ
    ~BitmapImage();

    std::unique_ptr<ImageCanvas> ReadBmp(const std::string filename);
    std::unique_ptr<ImageCanvas> ReadBmp(const char* filename);
    void WriteBmp(const std::string filename, ImageCanvas &canvas);
    void WriteBmp(const char* filename, ImageCanvas &canvas);

    void PrintBmpInfo(const std::string filename);
    void PrintBmpInfo(const char *filename);

  };
};
