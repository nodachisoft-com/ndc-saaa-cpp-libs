#include "PerlinNoise1234.hpp"

using namespace nl;

PerlinNoise1234::PerlinNoise1234()
    : isInit(false)
{
}

PerlinNoise1234::PerlinNoise1234(int _randseed)
    : isInit(false)
{
  init(_randseed);
}

bool PerlinNoise1234::init(int _randseed)
{
  if (isInit)
  {
    return false;
  }
  randseed = _randseed;
  MtRandomizer rand(_randseed);

  for (int i = 0; i < 255; i++)
  {
    for (int d = 0; d < 10; d++)
    {
      int index1 = rand.getRndInt(0, 255);
      int index2 = rand.getRndInt(0, 255);
      if (index1 != index2)
      {
        // index1 と index2 の配列内容を交換
        unsigned char memory = perm[index1];
        perm[index1] = perm[index2];
        perm[index2] = memory;
      }
    }
  }

  // [0,256-1]番目の配列を[256,512-1]番目の配列にコピー
  for (int i = 0; i < 256; i++)
  {
    perm[i + 256] = perm[i];
  }

  isInit = true; // 初期化完了
  return true;
}

float PerlinNoise1234::grad1(int hash, float x)
{
  int h = hash & 15;
  float grad = 1.0f + (h & 7); // Gradient value 1.0, 2.0, ..., 8.0
  if (h & 8)
    grad = -grad;    // and a random sign for the gradient
  return (grad * x); // Multiply the gradient with the distance
}

float PerlinNoise1234::grad2(int hash, float x, float y)
{
  int h = hash & 7;        // Convert low 3 bits of hash code
  float u = h < 4 ? x : y; // into 8 simple gradient directions,
  float v = h < 4 ? y : x; // and compute the dot product with (x,y).
  return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float PerlinNoise1234::grad3(int hash, float x, float y, float z)
{
  int h = hash & 15;       // Convert low 4 bits of hash code into 12 simple
  float u = h < 8 ? x : y; // gradient directions, and compute dot product.
  float v = h < 4 ? y : h == 12 || h == 14 ? x
                                           : z; // Fix repeats at h = 12 to 15
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

float PerlinNoise1234::grad4(int hash, float x, float y, float z, float t)
{
  int h = hash & 31;        // Convert low 5 bits of hash code into 32 simple
  float u = h < 24 ? x : y; // gradient directions, and compute dot product.
  float v = h < 16 ? y : z;
  float w = h < 8 ? z : t;
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v) + ((h & 4) ? -w : w);
}

//---------------------------------------------------------------------
/** 1D float Perlin noise, SL "noise()"
 */
float PerlinNoise1234::noise1(float x)
{
  int ix0, ix1;
  float fx0, fx1;
  float s, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  fx0 = x - ix0;      // Fractional part of x
  fx1 = fx0 - 1.0f;
  ix1 = (ix0 + 1) & 0xff;
  ix0 = ix0 & 0xff; // Wrap to 0..255

  s = fade(fx0);

  n0 = grad1(perm[ix0], fx0);
  n1 = grad1(perm[ix1], fx1);
  return 0.188f * (lerp(s, n0, n1));
}

//---------------------------------------------------------------------
/** 1D float Perlin periodic noise, SL "pnoise()"
 */
float PerlinNoise1234::pnoise1(float x, int px)
{
  int ix0, ix1;
  float fx0, fx1;
  float s, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  fx0 = x - ix0;      // Fractional part of x
  fx1 = fx0 - 1.0f;
  ix1 = ((ix0 + 1) % px) & 0xff; // Wrap to 0..px-1 *and* wrap to 0..255
  ix0 = (ix0 % px) & 0xff;       // (because px might be greater than 256)

  s = fade(fx0);

  n0 = grad1(perm[ix0], fx0);
  n1 = grad1(perm[ix1], fx1);
  return 0.188f * (lerp(s, n0, n1));
}

//---------------------------------------------------------------------
/** 2D float Perlin noise.
 */
float PerlinNoise1234::noise2(float x, float y)
{
  int ix0, iy0, ix1, iy1;
  float fx0, fy0, fx1, fy1;
  float s, t, nx0, nx1, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  iy0 = fastfloor(y); // Integer part of y
  fx0 = x - ix0;      // Fractional part of x
  fy0 = y - iy0;      // Fractional part of y
  fx1 = fx0 - 1.0f;
  fy1 = fy0 - 1.0f;
  ix1 = (ix0 + 1) & 0xff; // Wrap to 0..255
  iy1 = (iy0 + 1) & 0xff;
  ix0 = ix0 & 0xff;
  iy0 = iy0 & 0xff;

  t = fade(fy0);
  s = fade(fx0);

  nx0 = grad2(perm[ix0 + perm[iy0]], fx0, fy0);
  nx1 = grad2(perm[ix0 + perm[iy1]], fx0, fy1);
  n0 = lerp(t, nx0, nx1);

  nx0 = grad2(perm[ix1 + perm[iy0]], fx1, fy0);
  nx1 = grad2(perm[ix1 + perm[iy1]], fx1, fy1);
  n1 = lerp(t, nx0, nx1);

  return 0.507f * (lerp(s, n0, n1));
}

//---------------------------------------------------------------------
/** 2D float Perlin periodic noise.
 */
float PerlinNoise1234::pnoise2(float x, float y, int px, int py, float genHeight)
{
  int ix0, iy0, ix1, iy1;
  float fx0, fy0, fx1, fy1;
  float s, t, nx0, nx1, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  iy0 = fastfloor(y); // Integer part of y
  fx0 = x - ix0;      // Fractional part of x
  fy0 = y - iy0;      // Fractional part of y
  fx1 = fx0 - 1.0f;
  fy1 = fy0 - 1.0f;
  ix1 = ((ix0 + 1) % px) & 0xff; // Wrap to 0..px-1 and wrap to 0..255
  iy1 = ((iy0 + 1) % py) & 0xff; // Wrap to 0..py-1 and wrap to 0..255
  ix0 = (ix0 % px) & 0xff;
  iy0 = (iy0 % py) & 0xff;

  t = fade(fy0);
  s = fade(fx0);

  nx0 = grad2(perm[ix0 + perm[iy0]], fx0, fy0);
  nx1 = grad2(perm[ix0 + perm[iy1]], fx0, fy1);

  n0 = lerp(t, nx0, nx1);

  nx0 = grad2(perm[ix1 + perm[iy0]], fx1, fy0);
  nx1 = grad2(perm[ix1 + perm[iy1]], fx1, fy1);
  n1 = lerp(t, nx0, nx1);

  // lerp(s,n0,n1) は -1.512　～ 1.512 の範囲で結果を生成する
  // これを 0.0～genHeight の幅に射影する
  float res = ((1.511f + lerp(s, n0, n1)) / 3.025f) * genHeight;
  return res;
}

//---------------------------------------------------------------------
/** 3D float Perlin noise.
 */
float PerlinNoise1234::noise3(float x, float y, float z)
{
  int ix0, iy0, ix1, iy1, iz0, iz1;
  float fx0, fy0, fz0, fx1, fy1, fz1;
  float s, t, r;
  float nxy0, nxy1, nx0, nx1, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  iy0 = fastfloor(y); // Integer part of y
  iz0 = fastfloor(z); // Integer part of z
  fx0 = x - ix0;      // Fractional part of x
  fy0 = y - iy0;      // Fractional part of y
  fz0 = z - iz0;      // Fractional part of z
  fx1 = fx0 - 1.0f;
  fy1 = fy0 - 1.0f;
  fz1 = fz0 - 1.0f;
  ix1 = (ix0 + 1) & 0xff; // Wrap to 0..255
  iy1 = (iy0 + 1) & 0xff;
  iz1 = (iz0 + 1) & 0xff;
  ix0 = ix0 & 0xff;
  iy0 = iy0 & 0xff;
  iz0 = iz0 & 0xff;

  r = fade(fz0);
  t = fade(fy0);
  s = fade(fx0);

  nxy0 = grad3(perm[ix0 + perm[iy0 + perm[iz0]]], fx0, fy0, fz0);
  nxy1 = grad3(perm[ix0 + perm[iy0 + perm[iz1]]], fx0, fy0, fz1);
  nx0 = lerp(r, nxy0, nxy1);

  nxy0 = grad3(perm[ix0 + perm[iy1 + perm[iz0]]], fx0, fy1, fz0);
  nxy1 = grad3(perm[ix0 + perm[iy1 + perm[iz1]]], fx0, fy1, fz1);
  nx1 = lerp(r, nxy0, nxy1);

  n0 = lerp(t, nx0, nx1);

  nxy0 = grad3(perm[ix1 + perm[iy0 + perm[iz0]]], fx1, fy0, fz0);
  nxy1 = grad3(perm[ix1 + perm[iy0 + perm[iz1]]], fx1, fy0, fz1);
  nx0 = lerp(r, nxy0, nxy1);

  nxy0 = grad3(perm[ix1 + perm[iy1 + perm[iz0]]], fx1, fy1, fz0);
  nxy1 = grad3(perm[ix1 + perm[iy1 + perm[iz1]]], fx1, fy1, fz1);
  nx1 = lerp(r, nxy0, nxy1);

  n1 = lerp(t, nx0, nx1);

  return 0.936f * (lerp(s, n0, n1));
}

//---------------------------------------------------------------------
/** 3D float Perlin periodic noise.
 */
float PerlinNoise1234::pnoise3(float x, float y, float z, int px, int py, int pz)
{
  int ix0, iy0, ix1, iy1, iz0, iz1;
  float fx0, fy0, fz0, fx1, fy1, fz1;
  float s, t, r;
  float nxy0, nxy1, nx0, nx1, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  iy0 = fastfloor(y); // Integer part of y
  iz0 = fastfloor(z); // Integer part of z
  fx0 = x - ix0;      // Fractional part of x
  fy0 = y - iy0;      // Fractional part of y
  fz0 = z - iz0;      // Fractional part of z
  fx1 = fx0 - 1.0f;
  fy1 = fy0 - 1.0f;
  fz1 = fz0 - 1.0f;
  ix1 = ((ix0 + 1) % px) & 0xff; // Wrap to 0..px-1 and wrap to 0..255
  iy1 = ((iy0 + 1) % py) & 0xff; // Wrap to 0..py-1 and wrap to 0..255
  iz1 = ((iz0 + 1) % pz) & 0xff; // Wrap to 0..pz-1 and wrap to 0..255
  ix0 = (ix0 % px) & 0xff;
  iy0 = (iy0 % py) & 0xff;
  iz0 = (iz0 % pz) & 0xff;

  r = fade(fz0);
  t = fade(fy0);
  s = fade(fx0);

  nxy0 = grad3(perm[ix0 + perm[iy0 + perm[iz0]]], fx0, fy0, fz0);
  nxy1 = grad3(perm[ix0 + perm[iy0 + perm[iz1]]], fx0, fy0, fz1);
  nx0 = lerp(r, nxy0, nxy1);

  nxy0 = grad3(perm[ix0 + perm[iy1 + perm[iz0]]], fx0, fy1, fz0);
  nxy1 = grad3(perm[ix0 + perm[iy1 + perm[iz1]]], fx0, fy1, fz1);
  nx1 = lerp(r, nxy0, nxy1);

  n0 = lerp(t, nx0, nx1);

  nxy0 = grad3(perm[ix1 + perm[iy0 + perm[iz0]]], fx1, fy0, fz0);
  nxy1 = grad3(perm[ix1 + perm[iy0 + perm[iz1]]], fx1, fy0, fz1);
  nx0 = lerp(r, nxy0, nxy1);

  nxy0 = grad3(perm[ix1 + perm[iy1 + perm[iz0]]], fx1, fy1, fz0);
  nxy1 = grad3(perm[ix1 + perm[iy1 + perm[iz1]]], fx1, fy1, fz1);
  nx1 = lerp(r, nxy0, nxy1);

  n1 = lerp(t, nx0, nx1);

  return 0.936f * (lerp(s, n0, n1));
}

//---------------------------------------------------------------------
/** 4D float Perlin noise.
 */

float PerlinNoise1234::noise4(float x, float y, float z, float w)
{
  int ix0, iy0, iz0, iw0, ix1, iy1, iz1, iw1;
  float fx0, fy0, fz0, fw0, fx1, fy1, fz1, fw1;
  float s, t, r, q;
  float nxyz0, nxyz1, nxy0, nxy1, nx0, nx1, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  iy0 = fastfloor(y); // Integer part of y
  iz0 = fastfloor(z); // Integer part of y
  iw0 = fastfloor(w); // Integer part of w
  fx0 = x - ix0;      // Fractional part of x
  fy0 = y - iy0;      // Fractional part of y
  fz0 = z - iz0;      // Fractional part of z
  fw0 = w - iw0;      // Fractional part of w
  fx1 = fx0 - 1.0f;
  fy1 = fy0 - 1.0f;
  fz1 = fz0 - 1.0f;
  fw1 = fw0 - 1.0f;
  ix1 = (ix0 + 1) & 0xff; // Wrap to 0..255
  iy1 = (iy0 + 1) & 0xff;
  iz1 = (iz0 + 1) & 0xff;
  iw1 = (iw0 + 1) & 0xff;
  ix0 = ix0 & 0xff;
  iy0 = iy0 & 0xff;
  iz0 = iz0 & 0xff;
  iw0 = iw0 & 0xff;

  q = fade(fw0);
  r = fade(fz0);
  t = fade(fy0);
  s = fade(fx0);

  nxyz0 = grad4(perm[ix0 + perm[iy0 + perm[iz0 + perm[iw0]]]], fx0, fy0, fz0, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy0 + perm[iz0 + perm[iw1]]]], fx0, fy0, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix0 + perm[iy0 + perm[iz1 + perm[iw0]]]], fx0, fy0, fz1, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy0 + perm[iz1 + perm[iw1]]]], fx0, fy0, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx0 = lerp(r, nxy0, nxy1);

  nxyz0 = grad4(perm[ix0 + perm[iy1 + perm[iz0 + perm[iw0]]]], fx0, fy1, fz0, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy1 + perm[iz0 + perm[iw1]]]], fx0, fy1, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix0 + perm[iy1 + perm[iz1 + perm[iw0]]]], fx0, fy1, fz1, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy1 + perm[iz1 + perm[iw1]]]], fx0, fy1, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx1 = lerp(r, nxy0, nxy1);

  n0 = lerp(t, nx0, nx1);

  nxyz0 = grad4(perm[ix1 + perm[iy0 + perm[iz0 + perm[iw0]]]], fx1, fy0, fz0, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy0 + perm[iz0 + perm[iw1]]]], fx1, fy0, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix1 + perm[iy0 + perm[iz1 + perm[iw0]]]], fx1, fy0, fz1, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy0 + perm[iz1 + perm[iw1]]]], fx1, fy0, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx0 = lerp(r, nxy0, nxy1);

  nxyz0 = grad4(perm[ix1 + perm[iy1 + perm[iz0 + perm[iw0]]]], fx1, fy1, fz0, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy1 + perm[iz0 + perm[iw1]]]], fx1, fy1, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix1 + perm[iy1 + perm[iz1 + perm[iw0]]]], fx1, fy1, fz1, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy1 + perm[iz1 + perm[iw1]]]], fx1, fy1, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx1 = lerp(r, nxy0, nxy1);

  n1 = lerp(t, nx0, nx1);

  return 0.87f * (lerp(s, n0, n1));
}

//---------------------------------------------------------------------
/** 4D float Perlin periodic noise.
 */

float PerlinNoise1234::pnoise4(float x, float y, float z, float w,
                               int px, int py, int pz, int pw)
{
  int ix0, iy0, iz0, iw0, ix1, iy1, iz1, iw1;
  float fx0, fy0, fz0, fw0, fx1, fy1, fz1, fw1;
  float s, t, r, q;
  float nxyz0, nxyz1, nxy0, nxy1, nx0, nx1, n0, n1;

  ix0 = fastfloor(x); // Integer part of x
  iy0 = fastfloor(y); // Integer part of y
  iz0 = fastfloor(z); // Integer part of y
  iw0 = fastfloor(w); // Integer part of w
  fx0 = x - ix0;      // Fractional part of x
  fy0 = y - iy0;      // Fractional part of y
  fz0 = z - iz0;      // Fractional part of z
  fw0 = w - iw0;      // Fractional part of w
  fx1 = fx0 - 1.0f;
  fy1 = fy0 - 1.0f;
  fz1 = fz0 - 1.0f;
  fw1 = fw0 - 1.0f;
  ix1 = ((ix0 + 1) % px) & 0xff; // Wrap to 0..px-1 and wrap to 0..255
  iy1 = ((iy0 + 1) % py) & 0xff; // Wrap to 0..py-1 and wrap to 0..255
  iz1 = ((iz0 + 1) % pz) & 0xff; // Wrap to 0..pz-1 and wrap to 0..255
  iw1 = ((iw0 + 1) % pw) & 0xff; // Wrap to 0..pw-1 and wrap to 0..255
  ix0 = (ix0 % px) & 0xff;
  iy0 = (iy0 % py) & 0xff;
  iz0 = (iz0 % pz) & 0xff;
  iw0 = (iw0 % pw) & 0xff;

  q = fade(fw0);
  r = fade(fz0);
  t = fade(fy0);
  s = fade(fx0);

  nxyz0 = grad4(perm[ix0 + perm[iy0 + perm[iz0 + perm[iw0]]]], fx0, fy0, fz0, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy0 + perm[iz0 + perm[iw1]]]], fx0, fy0, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix0 + perm[iy0 + perm[iz1 + perm[iw0]]]], fx0, fy0, fz1, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy0 + perm[iz1 + perm[iw1]]]], fx0, fy0, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx0 = lerp(r, nxy0, nxy1);

  nxyz0 = grad4(perm[ix0 + perm[iy1 + perm[iz0 + perm[iw0]]]], fx0, fy1, fz0, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy1 + perm[iz0 + perm[iw1]]]], fx0, fy1, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix0 + perm[iy1 + perm[iz1 + perm[iw0]]]], fx0, fy1, fz1, fw0);
  nxyz1 = grad4(perm[ix0 + perm[iy1 + perm[iz1 + perm[iw1]]]], fx0, fy1, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx1 = lerp(r, nxy0, nxy1);

  n0 = lerp(t, nx0, nx1);

  nxyz0 = grad4(perm[ix1 + perm[iy0 + perm[iz0 + perm[iw0]]]], fx1, fy0, fz0, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy0 + perm[iz0 + perm[iw1]]]], fx1, fy0, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix1 + perm[iy0 + perm[iz1 + perm[iw0]]]], fx1, fy0, fz1, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy0 + perm[iz1 + perm[iw1]]]], fx1, fy0, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx0 = lerp(r, nxy0, nxy1);

  nxyz0 = grad4(perm[ix1 + perm[iy1 + perm[iz0 + perm[iw0]]]], fx1, fy1, fz0, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy1 + perm[iz0 + perm[iw1]]]], fx1, fy1, fz0, fw1);
  nxy0 = lerp(q, nxyz0, nxyz1);

  nxyz0 = grad4(perm[ix1 + perm[iy1 + perm[iz1 + perm[iw0]]]], fx1, fy1, fz1, fw0);
  nxyz1 = grad4(perm[ix1 + perm[iy1 + perm[iz1 + perm[iw1]]]], fx1, fy1, fz1, fw1);
  nxy1 = lerp(q, nxyz0, nxyz1);

  nx1 = lerp(r, nxy0, nxy1);

  n1 = lerp(t, nx0, nx1);

  return 0.87f * (lerp(s, n0, n1));
}
