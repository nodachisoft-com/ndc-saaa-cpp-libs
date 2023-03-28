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
}

void MapEngine::generateNewWorld()
{
  terrainEngine.init(mapConfig.terrainConfig, *dbRef);
}
