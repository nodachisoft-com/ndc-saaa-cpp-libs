#include "MapConfig.hpp"

using namespace a3c;

MapConfig::MapConfig()
{
  terrainConfig = TerrainConfig();
};

void MapConfig::init(const TerrainConfig _terrainConfig)
{
  terrainConfig = _terrainConfig;
}
