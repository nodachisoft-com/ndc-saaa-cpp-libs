/**
 * @brief
 *  Voronoi Diagram を元に 2D の位置に応じた type を生成する
 *  機能を提供する
 */
#pragma once
#include "../random/index.hpp"
#include <iostream>

namespace nl
{

  class NdcVoronoi
  {
  private:
    bool isInit;
    int randomSeed;
    int width;
    int height;
    struct Point
    {
      float x;
      float y;
      unsigned char type;
    };
    Point *point;

    /// @brief 高速用の inline floor 計算用
    /// @param x 計算対象の値
    /// @return floor 結果
    int fastfloor(float x)
    {
      return (((int)(x) <= (x)) ? ((int)x) : ((int)x - 1));
    }

    /// @brief
    ///   inline での要素計算用メソッド。
    ///   Point の２次元配列相当の位置を計算する
    ///   要素の位置は (width, height) で mod 計算されループする
    /// @param x 位置X
    /// @param y 位置Y
    /// @return インデックスの番号
    int calcPosIndex(int x, int y)
    {
      return (x % width) + (y % height) * width;
    }

    /// @brief inline メソッドにて 2 点間の距離を 2 乗した値を高速に求める
    /// @param x1 点1 の X 成分
    /// @param y1 点1 の y 成分
    /// @param x2 点2 の x 成分
    /// @param y2 点2 の y 成分
    /// @return 2 点の距離を 2 乗した結果
    float dist_pow2(float x1, float y1, float x2, float y2)
    {
      return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

  public:
    /// @brief デフォルトコンストラクタ定義
    NdcVoronoi(void);

    /// @brief Voronoi 図形の定義を設定し初期化する。px * py * Point サイズのメモリを使用する。
    /// @param randomSeed 乱数を定義
    /// @param typeRange Voronoi の計算点がもつ種類 [0, typeRange] の範囲で乱数決定
    /// @param[in] px ループする X 方向の幅（Ex:256）
    /// @param[in] py ループする Y 方向の高さ（Ex:256）
    NdcVoronoi(const int randomSeed, const unsigned char typeRange, int px, int py);

    /// @brief Voronoi 図形の定義を設定し初期化する。px * py * Point サイズのメモリを使用する。
    /// @param randomSeed 乱数を定義
    /// @param typeRange Voronoi の計算点がもつ種類 [0, typeRange] の範囲で乱数決定
    /// @param[in] px ループする X 方向の幅（Ex:256）
    /// @param[in] py ループする Y 方向の高さ（Ex:256）
    /// @return 正常に初期化完了で true。初期化済みだったり異常終了した場合は false
    bool init(const int randomSeed, const unsigned char typeRange, int px, int py);

    ~NdcVoronoi();

    /// @brief メモリ解放処理を行う。init処理時にも必要であれば事前に呼び出しをする
    void free();

    /// @brief Voronoi Diagram Noise 2D
    /// @param[in] x 位置の指定。 [0.0-px) でループ
    /// @param[in] y 位置の指定。 [0.0-py) でループ
    /// @return 0 ～ typeRange ノイズ生成の結果
    unsigned char pos2(float x, float y);
  };
};
