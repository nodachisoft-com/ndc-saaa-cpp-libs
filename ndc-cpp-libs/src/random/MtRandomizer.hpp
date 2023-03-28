/**
 * @file MtRandomizer.hpp
 * @brief MT19937 アルゴリズムを用いた乱数の生成、機能を提供する
 */
#pragma once
#include <random>

namespace nl
{

  /**
   * @brief
   *  MT19937 アルゴリズムを用いた乱数の生成、機能を提供する。
   *  IRandomizer インターフェイスにキャストしての使用を想定。
   */
  class MtRandomizer
  {
  public:
    /// @brief 乱数生成の種を設定
    int seed;

    /// @brief C++標準ライブラリ実装の MT19937 乱数生成器を使用
    std::mt19937 mt;

    /// @brief 乱数生成器の初期化を行う
    /// @param[in] seed 乱数生成の種を指定
    MtRandomizer(int seed);

    /// @brief [from,to) の範囲で生成する
    float getRndFloat(float from, float to);

    /// @brief [from,to] の範囲で生成する
    int getRndInt(int from, int to);
  };
};
