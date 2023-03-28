#pragma once
#include <cmath>
#include "Point2d.hpp"

namespace nl
{
  /// @brief
  ///   点 A(x,y)-点 B(x,y) を繋ぐ線分を定義
  class LineSegment2d
  {
  public:
    Point2d a;
    Point2d b;
    LineSegment2d(Point2d a, Point2d b) : a(a), b(b) {}
    float length()
    {
      return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
  };
}
