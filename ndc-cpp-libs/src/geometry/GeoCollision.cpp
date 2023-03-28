#include "GeoCollision.hpp"

using namespace nl;

float GeoCollision::powOfDistance_Point_To_LineSegment(const float px, const float py,
                                                       const float t1x, const float t1y,
                                                       const float t2x, const float t2y)
{
  float A = px - t1x;
  float B = py - t1y;
  float C = t2x - t1x;
  float D = t2y - t1y;

  float dot = A * C + B * D;
  float len_sq = C * C + D * D;
  float param = dot / len_sq;

  float xx, yy;

  if (param < 0)
  {
    xx = t1x;
    yy = t1y;
  }
  else if (param > 1)
  {
    xx = t2x;
    yy = t2y;
  }
  else
  {
    xx = t1x + param * C;
    yy = t1y + param * D;
  }

  float dx = px - xx;
  float dy = py - yy;
  return dx * dx + dy * dy;
}

bool GeoCollision::isInsideOfDistance_Point_To_LineSegment(const float px, const float py,
                                                           const float t1x, const float t1y,
                                                           const float t2x, const float t2y, const float condDistance)
{
  float powDistance = powOfDistance_Point_To_LineSegment(px, py, t1x, t1y, t2x, t2y);
  bool isInside = condDistance * condDistance >= powDistance;
  return isInside;
}

float GeoCollision::powOfDistance_Point_To_Circle(const float px, const float py,
                                                  const float cx, const float cy,
                                                  const float r)
{
  float dx = px - cx;
  float dy = py - cy;
  float powDistance = dx * dx + dy * dy - r * r;
  return powDistance;
}

bool GeoCollision::isInsideOfDistance_Point_To_Circle(const float px, const float py,
                                                      const float cx, const float cy,
                                                      const float r)
{
  float powDistance = powOfDistance_Point_To_Circle(px, py, cx, cy, r);
  bool isInside = 0 <= powDistance;
  return isInside;
}
