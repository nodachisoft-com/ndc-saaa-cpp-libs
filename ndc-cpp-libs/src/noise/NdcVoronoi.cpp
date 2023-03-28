#include "NdcVoronoi.hpp"

using namespace nl;

NdcVoronoi::NdcVoronoi(void)
    : isInit(false), point(nullptr)
{
  isInit = false;
  point = nullptr;
}
NdcVoronoi::NdcVoronoi(const int _randomSeed, const unsigned char typeRange, int px, int py)
    : isInit(false), point(nullptr)
{
  init(_randomSeed, typeRange, px, py);
}

bool NdcVoronoi::init(const int _randomSeed, const unsigned char typeRange, int px, int py)
{
  if (isInit)
  {
    // 既に初期化済み
    return false;
  }
  randomSeed = _randomSeed;
  MtRandomizer rand(_randomSeed);
  width = px;
  height = py;

  // Voronoi の計算用の格子点の位置を作成
  point = new Point[width * height];
  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      // 隣接する点を超えない範囲で乱数で位置に変化を加える
      int index = calcPosIndex(u, v); // v * px + u;
      point[index].x = rand.getRndFloat(-0.48f, 0.48f);
      point[index].y = rand.getRndFloat(-0.48f, 0.48f);
      point[index].type = rand.getRndInt(0, typeRange);
    }
  }
  isInit = true; // 初期化完了フラグをたてる
  return true;
}

NdcVoronoi::~NdcVoronoi()
{
  free();
}

void NdcVoronoi::free()
{
  if (point != nullptr)
  {
    delete[] point;
    point = nullptr;
  }
}

unsigned char NdcVoronoi::pos2(float x, float y)
{
  while (x < 0)
  {
    x += width;
  }
  while (x >= width)
  {
    x -= width;
  }
  while (y < 0)
  {
    y += height;
  }
  while (y >= height)
  {
    y -= height;
  }

  // 最寄りの 4 点
  Point UL, UR, BL, BR;
  bool isRightLoop = false;
  bool isBottomLoop = false;

  int floorX = fastfloor(x);
  int floorY = fastfloor(y);

  if (floorX + 1 > width)
  {
    // 右側がループして、左端の点を判断に使用する必要あり
    isRightLoop = true;
  }
  if (floorY + 1 > height)
  {
    // 下側がループして、上端の点を判断に使用する必要あり
    isBottomLoop = true;
  }

  UL = point[calcPosIndex(floorX, floorY)];
  UR = point[calcPosIndex(floorX + 1, floorY)];
  BL = point[calcPosIndex(floorX, floorY + 1)];
  BR = point[calcPosIndex(floorX + 1, floorY + 1)];

  float leftX = (float)floorX;
  float upperY = (float)floorY;
  float rightX = (float)( isRightLoop ? width : floorX + 1 );
  float bottomY = (float)( isBottomLoop ? height : floorY + 1);

  float distUL = dist_pow2(x, y, leftX + UL.x, upperY + UL.y);
  float distUR = dist_pow2(x, y, rightX + UR.x, upperY + UR.y);
  float distBL = dist_pow2(x, y, leftX + BL.x, bottomY + BL.y);
  float distBR = dist_pow2(x, y, rightX + BR.x, bottomY + BR.y);

  // 4 点のうち最短のポイントを取得する
  float minDist = distUL;
  Point nearPoint = UL;
  if (distUR < minDist)
  {
    minDist = distUR;
    nearPoint = UR;
  }
  if (distBL < minDist)
  {
    minDist = distBL;
    nearPoint = BL;
  }
  if (distBR < minDist)
  {
    minDist = distBR;
    nearPoint = BR;
  }
  return nearPoint.type;
}
