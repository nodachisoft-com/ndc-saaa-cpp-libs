#include "NdcNoisedEdgeVoronoi.hpp"

using namespace nl;

NdcNoisedEdgeVoronoi::NdcNoisedEdgeVoronoi(void)
    : isInit(false)
{
  isInit = false;
}
NdcNoisedEdgeVoronoi::NdcNoisedEdgeVoronoi(const int _randomSeed, const unsigned char typeRange, int px, int py)
    : isInit(false)
{
  init(_randomSeed, typeRange, px, py);
}

bool NdcNoisedEdgeVoronoi::init(const int _randomSeed, const unsigned char typeRange, int px, int py)
{
  if (isInit)
  {
    // 既に初期化済み
    return false;
  }
  randomSeed = _randomSeed;
  width = px;
  height = py;

  voronoi.init(randomSeed, typeRange, px, py);
  xNoise1.init(randomSeed);
  xNoise2.init(randomSeed + 11);
  yNoise1.init(randomSeed + 111);
  yNoise2.init(randomSeed + 231);

  isInit = true; // 初期化完了フラグをたてる
  return true;
}

unsigned char NdcNoisedEdgeVoronoi::pos2(float x, float y)
{
  float xDelta1Range = 2.0f * 0.3f / width;
  float xDelta2Range = 2.0f * 0.7f / width;
  float yDelta1Range = 2.0f * 0.3f / height;
  float yDelta2Range = 2.0f * 0.7f / height;
  float xDelta1 = xNoise1.pnoise2(x * 2, y * 2, width, height, xDelta1Range) - xDelta1Range * 0.5f;
  float xDelta2 = xNoise2.pnoise2(x * 4, y * 4, width, height, xDelta2Range) - xDelta2Range * 0.5f;
  float yDelta1 = yNoise1.pnoise2(x * 2, y * 2, width, height, yDelta1Range) - yDelta1Range * 0.5f;
  float yDelta2 = yNoise2.pnoise2(x * 4, y * 4, width, height, yDelta2Range) - yDelta2Range * 0.5f;

  float xDelta = xDelta1 + xDelta2;
  float yDelta = yDelta1 + yDelta2;
  return voronoi.pos2(x + xDelta, y + yDelta);
}
