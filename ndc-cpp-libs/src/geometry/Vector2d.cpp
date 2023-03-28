#include <math.h>
#include "Vector2d.hpp"

using namespace nl;

Vector2d::Vector2d() : x(0.0f), y(0.0f) {}

Vector2d::Vector2d(const float x, const float y)
    : x(x), y(y) {}

Vector2d Vector2d::normalize() const
{
  float mag = magnitude();
  if (mag != 0.0f)
  {
    return *this / mag;
  }
  return Vector2d();
}

float Vector2d::magnitude() const
{
  return sqrtf(x * x + y * y);
}
