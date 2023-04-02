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
// #include "DebugFontData.hpp"
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

    //BitmapImage *fontImage = nullptr; // 文字描画のフォントデータ読み込み先
    // std::string fontPath("debugfont.bmp"); // 読み込むフォントデータのパス
    //int fontWidth = 11;  // フォントの幅サイズ(px)
    //int fontHeight = 21; // フォントの高さ(px)

    // bool initializeFontdata(); // フォントデータを読み込む




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

    /// @brief 色情報をバッファに書き込む
    /// @param[in] x 書き込む画像内の位置（左を原点とする）
    /// @param[in] y 書き込む画像内の位置（上を原点とする）
    /// @param[in] color 書き込む色情報（RGB）
    void set(int x, int y, ColorRGB &color);

    /// @brief 指定の画像位置から色情報を取得する
    /// @param[in] x 書き込む画像内の位置（左を原点とする）
    /// @param[in] y 書き込む画像内の位置（上を原点とする）
    /// @return 取得した色情報（RGB）
    ColorRGB get(int x, int y);

    /// @brief 画像の横方向ピクセル数を取得する
    long getWidth();

    /// @brief 画像の縦方向ピクセル数を取得する
    long getHeight();

    /// @brief 画像を指定の色で塗りつぶす
    /// @param[in] color 塗りつぶす色
    void clear(ColorRGB &color);

    /// @brief
    ///  等幅フォント（1文字当たり11px、高さ 21px）で文字を描画する。
    ///  利用する初回にフォント画像情報を読み込む。
    /// @param[in] x 文字を描画する開始位置 x 座標
    /// @param[in] y 文字を描画する開始位置 y 座標
    /// @param[in] text 描画する文字列
    /// @param[in] color フォントの色情報（RGB）
    void writeText(const int x, const int y, const std::string text, ColorRGB &color);

    /// @brief
    ///  等幅フォント（1文字当たり11px、高さ 21px）で１文字を描画する。
    ///  利用する初回にフォント画像情報を読み込む。
    /// @param[in] x 文字を描画する開始位置 x 座標
    /// @param[in] y 文字を描画する開始位置 y 座標
    /// @param[in] oneText 描画する文字
    /// @param[in] color フォントの色情報（RGB）
    void writeChar(const int x, const int y, const char oneText, ColorRGB &color);


    /// <summary>
    /// BitmapImage に保持されている ImageCanvas への参照を取得する
    /// </summary>
    /// <returns>ImageCanvas への参照</returns>
    ImageCanvas* getRefImageCanvas()
    {
      return imgp;
    }

    /// <summary>
    /// ImageCanvas 情報をコピーして BitmapImage にセットする
    /// </summary>
    /// <param name="imageCanvas">コピー元の ImageCanvas 参照</param>
    //void setDeepCopyOfImageCanvas(ImageCanvas &imageCanvas);

  };
};
