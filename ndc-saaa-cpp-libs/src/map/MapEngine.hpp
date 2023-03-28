#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include "MapConfig.hpp"
#include "../terrain/TerrainEngine.hpp"

using namespace nl;

namespace a3c
{
  /// @brief
  ///  Worldmap の管理を行う
  class MapEngine
  {
  private:
    /// @brief 初期化済みフラグ
    bool initialized;

    /// @brief DB参照
    DBs *dbRef;

    /// @brief 地形生成エンジン
    TerrainEngine terrainEngine;

    /// @brief マップ生成用の設定
    MapConfig mapConfig;

  public:
    MapEngine();

    void init(const MapConfig mapConfig, DBs &dbRef);

    void generateNewWorld();

    /// @brief TerrainEngine への参照を取得する
    /// @return
    TerrainEngine *getTerrainEngine()
    {
      return &terrainEngine;
    }
  };
}
