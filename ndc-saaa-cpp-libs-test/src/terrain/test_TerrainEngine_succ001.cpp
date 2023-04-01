#include "../../pch.h"

#include <ndclibs.hpp>
#include "../../ndc-saaa-cpp-libs/src/a3commonlib.hpp"


using namespace nl;
using namespace a3c;

TEST(TerrainEngine, generateNewBaseLayer_case001)
{

  TerrainBaseConfig conf;
  conf.Seed = 101;
  conf.NaturalBiomeTypes = 4;
  conf.BiomeDivisionSmallestCount = 3;
  conf.WorldSmallestMapunitSize = Size2d(256, 256);
  conf.WorldScale = 4;
  conf.mapEdgeWide = 0.4f;

  DBs DBs; // 読み込み専用 DB の宣言・初期化
  DBs.init();

  TerrainConfig terrainConf;
  terrainConf.init( conf );

  // テスト対象のエンジン初期化
  TerrainEngine engine;
  engine.init( terrainConf , DBs );

  // 地理情報の初期値生成
  engine.generateNewBaseLayer();

  

  TerrainBaseLayerGenerator *terrainGenerator = engine.getBaseLayerGenerator();

  // BiomeList の内容を取得
  std::vector<Biome> *refBiomeList = terrainGenerator->getBiomeList();

  // HF のデバッグ画像出力
  Memory2d<float> *hf = terrainGenerator->getStrategyMapHF();
  TestUtil::drawHfColorful(hf, TESTTMP_DIR + "TerrainEngine_generateNewBaseLayer_case001_HF.bmp");

  

  // EXPECT_EQ(0xed82cd11, result);
  // EXPECT_EQ(0xed82cd11, crc.getHash());
}
