#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include <string>
#include "TerrainBaseConfig.hpp"
#include "../Biome.hpp"
#include "../../DBTables/DBs.hpp"
#include "TerrainGeoLabelGenerator.hpp"

using namespace nl;

namespace a3c
{
  /// @brief 
  ///  地理データで乱数から生成される基礎データ（Height Field、Biome）を生成する。
  ///  １．大域用データとして２次元データを生成し、キャッシュしておく
  ///  ２．ピンポイントな指定の位置のデータを取得する API を公開する
  class TerrainBaseLayerGenerator
  {
  private:
    TerrainBaseConfig conf;

    /// @brief データベースへの参照
    DBs *dbRef;

    /// @brief 地理ラベル生成モジュール
    TerrainGeoLabelGenerator terrainGeoLabelGenerator;

    /// @brief Biome
    NdcNoisedEdgeVoronoi biom;

    /// @brief Terrain layer1 ( Strategy Use )
    PerlinNoise1234 layer1;

    /// @brief Terrain layer2 ( Strategy Use )
    PerlinNoise1234 layer2;

    /// @brief Terrain layer3 ( Strategy Use )
    PerlinNoise1234 layer3;

    /// @brief Terrain layer4 ( Strategy Use )
    PerlinNoise1234 layer4;

    /// @brief マップ縁を海に鎮めるフィルタ
    EdgeNoiseCurveFilter edgeFileter;

    /// @brief Strategy 計算用の大域マップデータ（HF）を生成する
    Memory2d<float> strategyMapHF;

    /// @brief Biome 番号を取得する。Voronoi 点にランダムに設定された値を設定する
    Memory2d<short> strategyMapBiomType;

    /// @brief Biome をユニークに識別する値を設定する。
    Memory2d<short> strategyMapBiomeId;

    /// @brief 一定の面積以上のラベルをもつ Biome を記録する
    std::vector<Biome> biomeList;

    //std::vector<

  public:
    TerrainBaseLayerGenerator();
    ~TerrainBaseLayerGenerator();

    /// @brief
    ///  マップ生成に必要な情報を config 形式で設定する。
    ///  マップ生成には generateBaseTerrain を別途呼び出しする必要がある
    /// @param conf マップ生成に必要な情報
    void init(TerrainBaseConfig conf, DBs &dbRef);

    /// @brief
    ///  基本的な HeightField、Biome、Named Place 情報を生成する
    ///  1) generateStrategyMapHF();       … HF 作成
    ///  2) generateStrategyMapBiomType(); … Voronoi ベースで BiomeNo 色分け２次元データを生成
    ///  3) generateStrategyMapBiomeId();  … 一定以上の領域をもつ Biome にユニークID割り当て
    ///  4) generateNamedPlaceNetwork();   … 名前付き場所ノードと NW を形成
    void generateBaseTerrain();


    /// @brief 指定したマップチップをメートル単位でピンポイントに指定して高さを取得する
    float getWorldmapchipHf(const float u, const float v);

  private:
    /// @brief 赤道を 0 度(=0.0f) とし、南極・北極を 90 度(=1.0f) とする。
    /// @param v 座標 0.0f ～ 1.0f でマップの Y 成分を指定する
    /// @return 0.0f ～ 1.0f で緯度を返す
    float calcLatitude(const float v)
    {
      if (v < 0.5f)
      {
        return (0.5f - v) * 2.0f;
      }
      return (v - 0.5f) * 2.0f;
    }

    float getLayerHfAsWorldmapchip_StrategyLevel(const float u, const float v);

    /// @brief StrategyMapHF を生成する
    void generateStrategyMapHF();

    /// @brief Strategy レベルで BiomeNo 割り振りデータを生成する
    void generateStrategyMapBiomType();

    /// @brief Biome 領域ごとのユニークな ID を識別し、領域などの特性を記録する
    void generateStrategyMapBiomeId();

    /// @brief Biome 領域の中心点にて、名前付き場所ノードを作成しノード同士を接続する
    void generateNamedPlaceNetwork();

  public:
    // ============= アクセサメソッド ===============

    /// @brief 大域用マップデータの HeightField への参照を取得する
    /// @return
    Memory2d<float> *getStrategyMapHF()
    {
      return &strategyMapHF;
    }

    /// @brief 大域用マップデータの MapBiomeType への参照を取得する
    /// @return 大域用マップデータの MapBiomeType への参照
    Memory2d<short> *getStrategyMapBiomType()
    {
      return &strategyMapBiomType;
    }

    /// @brief 大域用マップデータの MapBiomeType への参照を取得する
    /// @return 大域用マップデータの MapBiomeType への参照
    Memory2d<short> *getStrategyMapBiomeId()
    {
      return &strategyMapBiomeId;
    }

    /// @brief Biome リストへの参照を取得する
    /// @return Biome リストへの参照
    std::vector<Biome> *getBiomeList()
    {
      return &biomeList;
    }
  };
}
