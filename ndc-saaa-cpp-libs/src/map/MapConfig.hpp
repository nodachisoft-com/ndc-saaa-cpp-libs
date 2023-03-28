#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include "../terrain/TerrainEngine.hpp"

using namespace nl;

namespace a3c
{
  /// @brief
  ///  Map を生成するためのパラメータを定義
  class MapConfig
  {

  public:
    /// @brief 生成の種で利用
    int Seed;

    /// @brief 地形生成に関する Config を保持し TerrainEngine に渡される
    TerrainConfig terrainConfig;

  public:
    /// @brief Config をデフォルト値で初期化する
    MapConfig();

    void init(const TerrainConfig terrainConfig);
  };
}
