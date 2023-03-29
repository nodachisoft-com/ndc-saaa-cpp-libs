#include "TerrainConfig.hpp"

using namespace a3c;

TerrainConfig::TerrainConfig(): Seed(0)
{
  terrainBaseConfig = TerrainBaseConfig();
};

void TerrainConfig::init(const TerrainBaseConfig _terrainBaseConfig)
{
  terrainBaseConfig = _terrainBaseConfig;
}
