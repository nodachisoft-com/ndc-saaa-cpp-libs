#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include "baselayer/TerrainBaseConfig.hpp"

using namespace nl;

namespace a3c
{
  /// @brief
  ///  World Map の地形を生成するためのパラメータを定義
  class TerrainConfig
  {

  public:
    /// @brief 生成の種で利用
    int Seed;

    /// @brief 乱数の種から一意に生成可能な TerrainBaseLayer へ渡す Config を保持
    TerrainBaseConfig terrainBaseConfig;

  public:
    /// @brief Config をデフォルト値で初期化する
    TerrainConfig();

    void init(const TerrainBaseConfig terrainBaseConfig);
  };
}
