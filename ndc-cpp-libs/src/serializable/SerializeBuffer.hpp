#pragma once
#include <vector>

namespace nl
{

  class SerializeBuffer
  {
  private:
    std::vector<char> buffer;
    long readoffset;

  public:
    SerializeBuffer();

    void setOffsetToHead();

    SerializeBuffer *appendInt(int v);
    int readInt();

    SerializeBuffer *appendFloat(float v);
    float readFloat();
  };

}
