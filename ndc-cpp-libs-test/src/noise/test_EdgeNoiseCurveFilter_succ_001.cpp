#include "../../pch.h"

#include <math.h>
#include <string>

using namespace nl;

// Edge Noise 図形で画像を生成し、CRC32 チェックを行う
TEST(EdgeNoiseCurveFilter, pos2_case001)
{
  
  int width = 640, height = 640;
  ImageCanvas canvas(width, height);
  float edgeWide = 0.3f;
  BitmapImage image;
  // ImageCanvas* canvas = image.getRefImageCanvas();
  EdgeNoiseCurveFilter filter(edgeWide);

  Crc32 crc;
  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      unsigned char h = (unsigned char)(255.0f * filter.pos2(u / (float)width, v / (float)height));
      crc.calcUpdate(h);
      ColorRGB color{h, h, h};
      canvas.set(u, v, color);
    }
  }
  std::string path(TESTTMP_DIR + "EdgeNoiseCurveFilter_pos2_case001.bmp");
  image.WriteBmp(path, canvas);
  EXPECT_EQ(crc.getHash(), 1330352757); // 161513028); // データ本体部の CRC32
}
