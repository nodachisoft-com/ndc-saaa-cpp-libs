#include "TerrainConfig.hpp"

using namespace a3c;

TerrainConfig::TerrainConfig()
{
  terrainBaseConfig = TerrainBaseConfig();
};

void TerrainConfig::init(const TerrainBaseConfig _terrainBaseConfig)
{
  terrainBaseConfig = _terrainBaseConfig;
}
