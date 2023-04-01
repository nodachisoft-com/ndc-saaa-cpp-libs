#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include "TerrainConfig.hpp"
#include "baselayer/TerrainBaseLayerGenerator.hpp"
#include "locationnetwork/NamedLocationMgr.hpp"

namespace a3c
{
  class TerrainEngine
  {
    TerrainConfig terrainConfig;

    DBs *dbRef;

    /// @brief BaseLayer 生成用エンジン
    TerrainBaseLayerGenerator terrainBaseLayerGen;

    /// @brief 名前付き場所管理クラス
    NamedLocationMgr namedLocationMgr;

    /// @brief インスタンス初期化済みフラグ
    bool _initialized;

  public:
    /// @brief デフォルトコンストラクタ
    TerrainEngine();

    /// @brief
    ///   インスタンスに設定ファイルを渡し、メンバの各種 Generator に
    ///   必要な Config を渡す
    /// @param terrainConfig 地形生成用の Config
    void init(const TerrainConfig terrainConfig, DBs &dbRef);

    /// @brief 新しく BaseLayer を生成する
    void generateNewBaseLayer();

  public: // ==== アクセサメソッド ====
    TerrainBaseLayerGenerator *getBaseLayerGenerator()
    {
      return &terrainBaseLayerGen;
    }

    // 名前付き場所の管理クラスへの参照を取得
    NamedLocationMgr* getNamedLocationMgr()
    {
      return &namedLocationMgr;
    }

  };
}
