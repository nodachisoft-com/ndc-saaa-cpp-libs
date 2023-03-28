#include "MapEngine.hpp"

using namespace a3c;

MapEngine::MapEngine()
{
  initialized = false;
  dbRef = nullptr;
};

void MapEngine::init(const MapConfig _mapConfig, DBs &_dbRef)
{
  dbRef = &_dbRef;
  mapConfig = _mapConfig;

  // Terrain データの生成
  terrainEngine.init(mapConfig.terrainConfig, *dbRef);
}

void MapEngine::generateNewWorld()
{
  // Terrain データの生成・Named Place 作成、Named Place 同士をつなぐ Network 経路の作成
  terrainEngine.generateNewBaseLayer();

  // TODO: Voronoi ベースの NamedPlace の取得を行い、経路を作成する

}
