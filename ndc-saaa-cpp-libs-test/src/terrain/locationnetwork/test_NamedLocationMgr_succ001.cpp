#include "../../../pch.h"
#include <ndclibs.hpp>
#include "../../ndc-saaa-cpp-libs/src/a3commonlib.hpp"

#include <iostream>

using namespace nl;
using namespace a3c;

TEST(NamedLocationMgr, case001)
{
  TerrainBaseConfig conf;
  conf.Seed = 102;
  conf.NaturalBiomeTypes = 4;
  conf.BiomeDivisionSmallestCount = 3;
  conf.WorldSmallestMapunitSize = Size2d(256, 256);
  conf.WorldScale = 4;
  conf.mapEdgeWide = 0.4f;

  DBs DBs; // 読み込み専用 DB の宣言・初期化
  DBs.init();

  TerrainBaseLayerGenerator terrainGenerator;
  terrainGenerator.init(conf, DBs);
  terrainGenerator.generateBaseTerrain();



  /*
  // NamedLocation の Node 一覧を取得
  std::vector<NamedLocationNode> nodeList = terrainGenerator.


  // BiomeList の内容を取得
  std::vector<Biome>* refBiomeList = terrainGenerator.getBiomeList();

  // HF のデバッグ画像出力
  Memory2d<float>* hf = terrainGenerator.getStrategyMapHF();
  TestUtil::drawHfColorful(hf, TESTTMP_DIR + "getStrategyMapHF_001_HF.bmp");

  // BIOME のデバッグ画像出力
  Memory2d<short>* biomeType = terrainGenerator.getStrategyMapBiomType();
  TestUtil::drawBiomeNoColorful(
    biomeType, TESTTMP_DIR + "getStrategyMapHF_001_BiomeType.bmp", refBiomeList, 500);
    */

}
