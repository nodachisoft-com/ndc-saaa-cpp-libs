#include "fsqrt.hpp"
// #include "iostream"

using namespace nl;
/*
Fsqrt::Fsqrt()
{
  sqrt_tables = std::vector<std::vector<float>>(MAX_DIGITS);
  interval_sizes = std::vector<float>(MAX_DIGITS);
  for (int i = 0; i < MAX_DIGITS; ++i)
  {
    interval_sizes[i] = MAX_INPUT[i] / N[i];
    sqrt_tables[i].resize(N[i] + 1);

    for (int j = 0; j <= N[i]; ++j)
    {
      float x = j * interval_sizes[i];
      sqrt_tables[i][j] = std::sqrt(x);
      // DEBUG:
      // std::cout << "DIG=" << i << ", "
      //          << " diffno=" << j << ", in=" << x << ", sqrt=" << sqrt_tables[i][j] << std::endl;
    }
  }
}

float Fsqrt::fsqrt(float x)
{
  if (x < 0.0f)
  {
    return 0.0f;
  }
  else if (x >= MAX_INPUT[MAX_DIGITS - 1])
  {
    // キャッシュ範囲外
    return std::sqrt(x);
  }

  // std::cout << "===BEGIN====" << std::endl;
  int digits = 0;
  float input = x;
  if (input < 1.0f)
  {
    digits = 0;
  }
  else
  {
    digits = 1;
  }
  while (input >= 10.0f)
  {
    digits++;
    input /= 10.0f;
  }

  // for (int i = 0; i < MAX_DIGITS; i++)
  // {
  //   std::cout << "interval_sizes[" << i << "]=" << interval_sizes[i] << std::endl;
  //}

  int index = static_cast<int>(x / interval_sizes[digits]);
  float x0 = index * interval_sizes[digits];
  float x1 = (index + 1) * interval_sizes[digits];
  // int x0index = (int)x0;
  float y0 = sqrt_tables[digits][index];
  float y1 = sqrt_tables[digits][index + 1];
  float t = (x - x0) / (x1 - x0);

  // std::cout << "CALC:" << std::endl;
  // std::cout << "index=" << index << ", input=" << input << ", interval=" << interval_sizes[digits] << std::endl;
  // std::cout << "DIG=" << digits << ", diffno=" << index << ", in=" << x << ", sqrt=" << y0 << std::endl;

  return y0 * (1.0f - t) + y1 * t;
}
*/
