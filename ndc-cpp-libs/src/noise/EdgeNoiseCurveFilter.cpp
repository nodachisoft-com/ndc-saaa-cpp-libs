#include "EdgeNoiseCurveFilter.hpp"

using namespace nl;

EdgeNoiseCurveFilter::EdgeNoiseCurveFilter(void)
    : isInit(false)
{
  isInit = false;
}
EdgeNoiseCurveFilter::EdgeNoiseCurveFilter(const float edgeWidePerc)
    : isInit(false)
{
  init(edgeWidePerc);
}

bool EdgeNoiseCurveFilter::init(const float _edgeWidePerc)
{
  if (isInit)
  {
    // 既に初期化済み
    return false;
  }
  edgeWidePerc = _edgeWidePerc;

  isInit = true; // 初期化完了フラグをたてる
  return true;
}

float EdgeNoiseCurveFilter::pos2(const float x, const float y)
{
  float distXfromEdge = min(x, 1.0f - x);
  float distYfromEdge = min(y, 1.0f - y);
  float dist = min(distXfromEdge, distYfromEdge);
  if (dist >= edgeWidePerc)
  {
    return 1.0f;
  }

  float h = plerp(dist / edgeWidePerc);
  return h;
}
