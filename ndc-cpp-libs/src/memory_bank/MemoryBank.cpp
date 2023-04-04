#include "../exception/index.hpp"
#include "MemoryBank.hpp"
#include "../hash/index.hpp"

using namespace nl;

/**
 * コンストラクタ
 * @brief スケーラブルなバイト列操作用のクラスを初期化する
 */
MemoryBank::MemoryBank()
{
  memoryBlockSize = DEFAULT_MEMORY_BLOCK_SIZE;
}

MemoryBank::MemoryBank(int _memoryBlockSize)
{
  memoryBlockSize = _memoryBlockSize;
}

// デストラクタ。必要なだけ動的に確保したメモリブロックのスロットを解放する
MemoryBank::~MemoryBank()
{
  // 使用しているメモリスロットの参照先メモリを解放する
  size_t memorySize = memory.size();
  for (size_t slotNo = 0; slotNo < memorySize; slotNo++)
  {
    free(memory[slotNo]);
  }
}

unsigned char MemoryBank::get(const int index)
{
  if (index < 0 || index > endPos)
  {
    // メモリ範囲外へのアクセスが発生した
    // アクセス可能な要素の最大を計算
    const size_t accessableRangeLimit = memoryBlockSize * memory.size() - 1;
    std::stringstream msg;
    msg << "Access Violation offset=[" << index << "], Accessable Range=["
        << 0 << "," << accessableRangeLimit << "]" << std::flush;
    throw OutOfRangeAccessException(std::string(msg.str()));
  }
  int slotNo = index / memoryBlockSize;
  int offset = index % memoryBlockSize;
  unsigned char result = memory[slotNo][offset];
  return result;
}

void MemoryBank::set(const int index, const unsigned char value)
{
  if (index < 0 || index > endPos)
  {
    // メモリ範囲外へのアクセスが発生した
    // アクセス可能な要素の最大を計算
    const size_t accessableRangeLimit = memoryBlockSize * memory.size() - 1;
    std::stringstream msg;
    msg << "Access Violation offset=[" << index << "], Accessable Range=["
        << 0 << "," << accessableRangeLimit << "]" << std::flush;
    throw OutOfRangeAccessException(std::string(msg.str()));
  }
  int slotNo = index / memoryBlockSize;
  int offset = index % memoryBlockSize;
  memory[slotNo][offset] = value;
}

MemoryBank *MemoryBank::appendByte(const unsigned char value)
{
  int nextEndPos = endPos + 1;
  bool isBound = (nextEndPos % memoryBlockSize == 0 ? true : false);
  if (isBound)
  {
    // メモリブロックの末まで使用している状態のため、
    // 次のスロットにメモリを確保して使用可能にする
    if (useNewMemorySlot() == false)
    {
      throw OutOfMemoryException("OUT OF MEMORY!!");
    }
  }
  endPos++;
  set(endPos, value);
  return this;
}

template <typename X>
MemoryBank *MemoryBank::append(const X value)
{
  unsigned char *byteArray = (unsigned char *)(void *)&value;
  int size = (int)sizeof(X);
  for (int i = 0; i < size; i++)
  {
    appendByte(byteArray[i]);
  }
  return this;
}
template MemoryBank *MemoryBank::append<char>(char);
template MemoryBank *MemoryBank::append<short>(short);
template MemoryBank *MemoryBank::append<int>(int);
template MemoryBank *MemoryBank::append<long>(long);
template MemoryBank *MemoryBank::append<float>(float);
template MemoryBank *MemoryBank::append<double>(double);
template MemoryBank *MemoryBank::append<bool>(bool);

MemoryBank *MemoryBank::appendStringWithLength(const std::string &value)
{
  int len = (int)value.length();
  append(len);

  const char *buf = value.c_str();
  // 文字列本体を記録
  for (int i = 0; i < len; i++)
  {
    appendByte(buf[i]);
  }
  return this;
}

MemoryBank *MemoryBank::appendCharArrayWithLength(const char value[])
{
  int len = (int)strlen(value);
  append(len);

  //  文字列本体を記録
  for (int i = 0; i < len; i++)
  {
    appendByte(value[i]);
  }
  return this;
}

MemoryBank *MemoryBank::appendCharArray(const char value[])
{
  size_t len = strlen(value);

  //  文字列本体を記録
  for (size_t i = 0; i < len; i++)
  {
    appendByte(value[i]);
  }
  return this;
}

MemoryBank *MemoryBank::appendString(const std::string value)
{
  size_t len = value.size();
  const char *charArray = value.c_str();

  //  文字列本体を記録
  for (size_t i = 0; i < len; i++)
  {
    appendByte(charArray[i]);
  }
  return this;
}

template <typename X>
void MemoryBank::read(X &value)
{
  // short result = 0;
  char *mem = (char *)(void *)(&value);

  int size = (int)sizeof(X);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
}
template void MemoryBank::read<char>(char &v);
template void MemoryBank::read<short>(short &v);
template void MemoryBank::read<int>(int &v);
template void MemoryBank::read<long>(long &v);
template void MemoryBank::read<float>(float &v);
template void MemoryBank::read<double>(double &v);
template void MemoryBank::read<bool>(bool &v);

// 読み取りカーソル位置をバッファの頭にセット
void MemoryBank::setCurPosToHead()
{
  curPos = 0;
}

// 現在のカーソル位置から char を読み込み、カーソル位置を進める
char MemoryBank::readChar()
{
  char result = get(curPos);
  curPos++;
  return result;
}

// 現在のカーソル位置から short を読み込み、カーソル位置を進める
short MemoryBank::readShort()
{
  short result = 0;
  char *mem = (char *)(void *)(&result);

  int size = (int)sizeof(short);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
  return result;
}

// 現在のカーソル位置から int を読み込み、カーソル位置を進める
int MemoryBank::readInt()
{
  int result = 0;
  char *mem = (char *)(void *)(&result);

  int size = (int)sizeof(int);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
  return result;
}

// 現在のカーソル位置から long を読み込み、カーソル位置を進める
long MemoryBank::readLong()
{
  long result = 0;
  char *mem = (char *)(void *)(&result);

  int size = (int)sizeof(long);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
  return result;
}

// 現在のカーソル位置から float を読み込み、カーソル位置を進める
float MemoryBank::readFloat()
{
  float result = 0;
  char *mem = (char *)(void *)(&result);

  int size = (int)sizeof(float);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
  return result;
}

// 現在のカーソル位置から double を読み込み、カーソル位置を進める
double MemoryBank::readDouble()
{
  double result = 0;
  char *mem = (char *)(void *)(&result);
  int size = (int)sizeof(double);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
  return result;
}

// 現在のカーソル位置から bool を読み込み、カーソル位置を進める
bool MemoryBank::readBool()
{
  bool result = false;
  char *mem = (char *)(void *)(&result);
  int size = (int)sizeof(bool);
  for (int i = 0; i < size; i++)
  {
    mem[i] = readChar();
  }
  return result;
}

// 現在のカーソル位置から char* を記録された長さぶん読み込み、
// 末尾に終端文字（\0）を付与して返す。読み取ったぶんのカーソル位置を進める
std::string MemoryBank::readStringWithLength()
{
  int length = readInt();
  char* newCharArray = new char[length + 1];
  //char newCharArray[length + 1];
  for (int i = 0; i < length; i++)
  {
    newCharArray[i] = readChar();
  }
  newCharArray[length] = '\0'; // 終端文字を追加
  std::string result(newCharArray);
  delete[] newCharArray;
  return result;
}

// MemoryBank のカーソル位置を先頭にセットし、すべてのメモリを文字列として読み込んだ後、
// 末尾に終端文字（\0）を付与して返す。読み取ったぶんのカーソル位置を進める
std::string MemoryBank::readStringToEnd()
{
  setCurPosToHead();

  long long length = getUsingSize();
  char *newCharArray;
  newCharArray = new char[length + 1];
  for (long long  i = 0; i < length; i++)
  {
    newCharArray[i] = readChar();
  }
  newCharArray[length] = '\0'; // 終端文字を追加
  std::string result(newCharArray);
  delete[] newCharArray; // 領域の解放
  return result;
}

// バイト列保存領域として確保しているメモリ使用量を取得する
// @return バッファのメモリ使用量
int MemoryBank::getAllocMemorySize()
{
  size_t usingMemorySlot = memory.size();
  size_t memorySize = usingMemorySlot * memoryBlockSize;
  return (int)memorySize;
}

// アクセス可能な割り当て済みバイト列のバイト数の長さ取得する。
// 3 Byte を保存済みなら 3 を返す
// 確保したメモリ容量ブロックはスロット単位であり、 getAllocMemorySize() とは
// 異なる結果となる
// @return バイト列の長さ。
int MemoryBank::getUsingSize()
{
  return endPos + 1;
}

// メモリの CRC32 を計算する
unsigned long MemoryBank::calcCrc32()
{
  Crc32 crc;
  int length = getUsingSize();
  for (int i = 0; i < length; i++)
  {
    crc.calcUpdate(get(i));
  }
  return crc.getHash();
}

// 使用しているメモリスロット数、メモリ容量などのデバッグ情報を出力します
void MemoryBank::debug()
{
  size_t usingMemorySlot = memory.size();
  size_t memorySize = usingMemorySlot * memoryBlockSize;
  std::cout
      << "[INFO] Using Memory Slot Count : " << usingMemorySlot << std::endl;
  std::cout << "[INFO] Using Memory Mount Size : " << memorySize << std::endl;
}

// ========= PRIVATE METHOD ===========
// 追加でメモリスロットにメモリを割り当て、領域をすべて 0 で初期化する
// @return 処理成功なら TRUE、メモリ確保に失敗した場合 FALSE を返す
bool MemoryBank::useNewMemorySlot()
{
  // char *newBlock = (char *)malloc(sizeof(char) * memoryBlockSize);
  unsigned char *newBlock = (unsigned char *)calloc(memoryBlockSize, sizeof(unsigned char));
  if (newBlock == NULL)
  {
    return false;
  }
  memory.push_back(newBlock);
  return true;
}
