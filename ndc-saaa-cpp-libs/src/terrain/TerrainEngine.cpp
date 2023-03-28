#include "TerrainEngine.hpp"

using namespace a3c;

TerrainEngine::TerrainEngine()
{
  // 初期化未済のフラグをたてる
  _initialized = false;
  dbRef = nullptr;
}

void TerrainEngine::init(const TerrainConfig _terrainConfig, DBs &_dbRef)
{
  terrainConfig = _terrainConfig;
  dbRef = &_dbRef;

  // TerrainBaseLayerGenerator を初期化
  baseLayerGen.init(terrainConfig.terrainBaseConfig, _dbRef);
  _initialized = true; // 初期化済みフラグをたてる
}

void TerrainEngine::generateNewBaseLayer()
{
  // BaseLayer の HF、Biome、NamedPlace のポイントを生成する
  baseLayerGen.generateBaseTerrain();
}
