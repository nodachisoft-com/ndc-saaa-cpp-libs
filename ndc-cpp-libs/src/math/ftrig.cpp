#include "ftrig.hpp"

using namespace nl;

Ftrig::Ftrig()
{
  cos_table = std::vector<float>(CACHE_SIZE);
  sin_table = std::vector<float>(CACHE_SIZE);

  interval_size = 2.0f * PI / CACHE_SIZE;
  for (int i = 0; i <= CACHE_SIZE; ++i)
  {
    float x = i * interval_size;
    cos_table[i] = std::cos(x);
    sin_table[i] = std::sin(x);
  }
}

float Ftrig::fsin(float x)
{
  x = std::fmod(x, 2.0f * PI);
  if (x < 0.0f)
  {
    x += 2.0f * PI;
  }
  int index = static_cast<int>(x / interval_size);
  float x0 = index * interval_size;
  float x1 = (index + 1) * interval_size;
  float y0 = sin_table[index];
  float y1 = sin_table[index + 1];
  float t = (x - x0) / (x1 - x0);
  return y0 * (1.0f - t) + y1 * t;
}

float Ftrig::fcos(float x)
{
  x = std::fmod(x, 2.0f * PI);
  if (x < 0.0f)
  {
    x += 2.0f * PI;
  }
  int index = static_cast<int>(x / interval_size);
  float x0 = index * interval_size;
  float x1 = (index + 1) * interval_size;
  float y0 = cos_table[index];
  float y1 = cos_table[index + 1];
  float t = (x - x0) / (x1 - x0);
  return y0 * (1.0f - t) + y1 * t;
}
