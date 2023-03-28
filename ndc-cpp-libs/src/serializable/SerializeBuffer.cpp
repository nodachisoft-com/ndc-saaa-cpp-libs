#include "../ndclibs.hpp"
#include "SerializeBuffer.hpp"

using namespace nl;

// コンストラクタ
SerializeBuffer::SerializeBuffer()
{
  buffer = {};
  readoffset = 0;
};

void SerializeBuffer::setOffsetToHead()
{
  readoffset = 0;
};

SerializeBuffer *SerializeBuffer::*appendInt(int v)
{
  // TODO buffer への追加処理を記述

  return NULL;
};

int SerializeBuffer::readInt()
{
  return 1;
}

SerializeBuffer *SerializeBuffer::*appendFloat(float v)
{
  // TODO buffer への追加処理を記述
  return NULL;
};

float SerializeBuffer::readFloat()
{
  return 1.0f;
}
