/**
 * @brief
 *  NdcVoronoi を使用して、 2D の位置に応じた type を生成する機能を提供する
 *  生成される Voronoi 図は PerlineNoise により連続性を持った乱数値で x,y 成分をずらして Voronoi の
 *  値を取得する
 */
#pragma once
#include <iostream>
#include "../random/index.hpp"
#include "NdcVoronoi.hpp"
#include "PerlinNoise1234.hpp"

namespace nl
{

  class NdcNoisedEdgeVoronoi
  {
  private:
    bool isInit;
    int randomSeed;
    int width;
    int height;

    NdcVoronoi voronoi;
    PerlinNoise1234 xNoise1;
    PerlinNoise1234 xNoise2;
    PerlinNoise1234 yNoise1;
    PerlinNoise1234 yNoise2;

  public:
    /// @brief デフォルトコンストラクタ定義
    NdcNoisedEdgeVoronoi(void);

    /// @brief Voronoi 図形の定義を設定し初期化する。px * py * Point サイズのメモリを使用する。
    /// @param randomSeed 乱数を定義
    /// @param typeRange Voronoi の計算点がもつ種類 [0, typeRange] の範囲で乱数決定
    /// @param[in] px ループする X 方向の幅（Ex:256）
    /// @param[in] py ループする Y 方向の高さ（Ex:256）
    NdcNoisedEdgeVoronoi(const int randomSeed, const unsigned char typeRange, int px, int py);

    /// @brief Voronoi 図形の定義を設定し初期化する。px * py * Point サイズのメモリを使用する。
    /// @param randomSeed 乱数を定義
    /// @param typeRange Voronoi の計算点がもつ種類 [0, typeRange] の範囲で乱数決定
    /// @param[in] px ループする X 方向の幅（Ex:256）
    /// @param[in] py ループする Y 方向の高さ（Ex:256）
    /// @return 正常に初期化完了で true。初期化済みだったり異常終了した場合は false
    bool init(const int randomSeed, const unsigned char typeRange, int px, int py);

    /// @brief Voronoi Diagram Noise 2D
    /// @param[in] x 位置の指定。 [0.0-px) でループ
    /// @param[in] y 位置の指定。 [0.0-py) でループ
    /// @return 0 ～ typeRange ノイズ生成の結果
    unsigned char pos2(float x, float y);
  };
};
