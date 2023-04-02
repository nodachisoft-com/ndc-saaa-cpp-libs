/**
 * @brief 画像のキャンバス情報を扱う
 */
#pragma once

#include <string>
#include <iostream>
#include "../memory_bank/Memory2d.hpp"
#include "DebugFontData.hpp"
#include "ColorRGB.hpp"

namespace nl
{

  /// @brief 2次元の画像キャンバス情報をもつクラス
  class ImageCanvas
  {
  public:

    /// @brief データ本体
    Memory2d<ColorRGB> data;

    /// @brief 共有されるフォントデータ
    static ImageCanvas *fontCanvas;

    /// <summary>
    /// ImageCanvas のインスタンス数に応じたカウント管理
    /// fontCanvas の1 回目呼び出し時の初期化、全削除判定に利用
    /// </summary>
    static int sharedCountForFontCanvas;

  public:
    ImageCanvas(const int width, const int height)
      : data(width, height, ColorRGB(0, 0, 0))
    {
      initDefaultFontData();
    }

    ~ImageCanvas()
    {
      sharedCountForFontCanvas--;
      if (sharedCountForFontCanvas == 0 )
      {
        // ImageCanvas オブジェクトの利用者が 0 になったら static 内容を削除
        delete fontCanvas;
      }
    }

    /// @brief 色情報をバッファに書き込む
    /// @param[in] x 書き込む画像内の位置（左を原点とする）
    /// @param[in] y 書き込む画像内の位置（上を原点とする）
    /// @param[in] color 書き込む色情報（RGB）
    void set(int x, int y, ColorRGB& color)
    {
      data.setWithIgnoreOutOfRangeData(x, y, color);
    }


    /// @brief 指定の画像位置から色情報を取得する
    /// @param[in] x 書き込む画像内の位置（左を原点とする）
    /// @param[in] y 書き込む画像内の位置（上を原点とする）
    /// @return 取得した色情報（RGB）
    ColorRGB get(int x, int y)
    {
      return data.getWithIgnoreOutOfRangeData(x, y);
    }

    /// @brief 画像を指定の色で塗りつぶす
    /// @param[in] color 塗りつぶす色
    void clear(ColorRGB& color)
    {
      data.setWholeData(color);
    }

    /// @brief
    ///  等幅フォント（1文字当たり11px、高さ 21px）で文字を描画する。
    ///  利用する初回にフォント画像情報を読み込む。
    /// @param[in] x 文字を描画する開始位置 x 座標
    /// @param[in] y 文字を描画する開始位置 y 座標
    /// @param[in] text 描画する文字列
    /// @param[in] color フォントの色情報（RGB）
    void writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB& color);

    /// @brief
    ///  等幅フォント（1文字当たり11px、高さ 21px）で１文字を描画する。
    ///  利用する初回にフォント画像情報を読み込む。
    /// @param[in] x 文字を描画する開始位置 x 座標
    /// @param[in] y 文字を描画する開始位置 y 座標
    /// @param[in] oneText 描画する文字
    /// @param[in] color フォントの色情報（RGB）
    void writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB& color);

    /// <summary>
    /// フォントデータを初期化する
    /// </summary>
    void initDefaultFontData();

  public: // アクセサ

    /// @brief 画像の横方向ピクセル数を取得する
    int getWidth()
    {
      return data.getWidth();
    }

    /// @brief 画像の縦方向ピクセル数を取得する
    int getHeight()
    {
      return data.getHeight();
    }
  };
};


