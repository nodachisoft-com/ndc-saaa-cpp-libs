#include <iostream>
#include <ndclibs.hpp>
#include "../../src/a3commonlib.hpp"

using namespace nl;
using namespace a3c;

int main(int argc, char **argv)
{
  BitmapImage image(100, 100);
  image.WriteBmp("test.bmp");
  return 0;
}
