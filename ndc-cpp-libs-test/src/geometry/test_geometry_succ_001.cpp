#include "../../pch.h"

using namespace nl;
// CRC32 コードが想定通りであること
TEST(GeoCollision, powOfDistance_Point_To_LineSegment_succ001)
{

  // p(0,1) と線分 (1,0)-(5,0) の最短距離は sqrt(1.0 + 1.0)。二乗したら 2.0
  float dist = GeoCollision::powOfDistance_Point_To_LineSegment(0, 1, 1, 0, 5, 0);
  EXPECT_EQ(2.0f, dist);
}

// 線分の近くを色分けする
TEST(GeoCollision, isInsideOfDistance_Point_To_LineSegment_succ001)
{
  BitmapImage image(100, 100);
  ImageCanvas *canvas = image.getRefImageCanvas();
  // 線分 (10,30)-(75,66) との距離が 7.0f 以内であるかを判定して色分けする
  bool isInside = false;
  Crc32 crc;
  for (int v = 0; v < 100; v++)
  {
    for (int u = 0; u < 100; u++)
    {
      isInside = GeoCollision::isInsideOfDistance_Point_To_LineSegment((float)u, (float)v, 10, 30, 75, 66, 7.0f);
      if (isInside)
      {
        // 茶色（道のイメージ）
        ColorRGB color{219, 182, 55};
        canvas->set(u, v, color);
        crc.calcUpdate('A');
      }
      else
      {
        // 青色
        ColorRGB color{0, 52, 192};
        canvas->set(u, v, color);
        crc.calcUpdate('X');
      }
    }
  }
  image.WriteBmp(TESTTMP_DIR + "test_isInsideOfDistance_Point_To_LineSegment_succ001.bmp");

  EXPECT_EQ(1255576523, crc.getHash());
}

// 円の近くを色分けする
TEST(GeoCollision, isInsideOfDistance_Point_To_Circle_succ001)
{
  BitmapImage image(100, 100);
  ImageCanvas* canvas = image.getRefImageCanvas();
  // 中心点 (30,40)、半径 27 の範囲とそれ以外を色分けする
  bool isInside = false;
  Crc32 crc;
  for (int v = 0; v < 100; v++)
  {
    for (int u = 0; u < 100; u++)
    {
      isInside = GeoCollision::isInsideOfDistance_Point_To_Circle((float)u, (float)v, 30, 40, 27.0f);
      if (isInside)
      {
        // 茶色（道のイメージ）
        ColorRGB color{219, 182, 55};
        canvas->set(u, v, color);
        crc.calcUpdate('A');
      }
      else
      {
        // 青色
        ColorRGB color{0, 52, 192};
        canvas->set(u, v, color);
        crc.calcUpdate('X');
      }
    }
  }
  image.WriteBmp(TESTTMP_DIR + "test_isInsideOfDistance_Point_To_Circle_succ001.bmp");

  EXPECT_EQ(499129887, crc.getHash());
}
