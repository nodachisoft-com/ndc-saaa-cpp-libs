/**
 * @brief デバッグ等に利用可能な Bitmap 画像形式の読み込み、書き込み機能を提供する
 */
#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "DebugFontData.hpp"

namespace nl
{
  /// @brief RGB形式の画素情報
  typedef struct
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
  } ColorRGB;

  /// @brief 画像データのデータ部
  typedef struct
  {
    int height;
    int width;
    ColorRGB *data;
  } ImageDataStruct;

  /// @brief Bitmap １つの画像ファイル、データを扱う
  class BitmapImage
  {
  private:
    // 定数
    static const int HEADERSIZE = 54;
    static const int PALLETSIZE = 1024;
    static const int MAX_IMAGE_MEMORY = (1024 * 1080) * (4 * 2); // @brief 4K レベルを扱い可能な上限サイズとする

    // メタ情報部
    // TODO: MetaInfo 構造体に入れ込むべき
    unsigned char Bmp_headbuf[54];  /// @brief ヘッダを格納するための変数
    unsigned char Bmp_Pallet[1024]; /// @brief カラーパレットを格納

    char Bmp_type[2];                  /// @brief ファイルタイプ "BM"
    unsigned long Bmp_size;            /// @brief bmpファイルのサイズ (バイト)
    unsigned int Bmp_info_header_size; /// @brief 情報ヘッダのサイズ = 40
    unsigned int Bmp_header_size;      /// @brief ヘッダサイズ = 54
    long Bmp_height;                   /// @brief 高さ (ピクセル)
    long Bmp_width;                    /// @brief 幅   (ピクセル)
    unsigned short Bmp_planes;         /// @brief プレーン数 常に 1
    unsigned short Bmp_color;          /// @brief 色 (ビット)     24
    long Bmp_comp;                     /// @brief 圧縮方法         0
    long Bmp_image_size;               /// @brief 画像部分のファイルサイズ (バイト)
    long Bmp_xppm;                     /// @brief 水平解像度 (ppm)
    long Bmp_yppm;                     /// @brief 垂直解像度 (ppm)
    ImageDataStruct imgp;              /// 画像データ本体

    BitmapImage *fontImage = NULL; // 文字描画のフォントデータ読み込み先
    // std::string fontPath("debugfont.bmp"); // 読み込むフォントデータのパス
    int fontWidth = 11;  // フォントの幅サイズ(px)
    int fontHeight = 21; // フォントの高さ(px)

    bool initializeFontdata(); // フォントデータを読み込む

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
  };
};
