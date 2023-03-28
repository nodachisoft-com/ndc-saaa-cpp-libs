#include "../../pch.h"

using namespace nl;
using namespace std::string_literals;

TEST(MemoryBank, succ_readWriteChar001)
{
    MemoryBank buf(5);
    buf.append((char)64);
    EXPECT_EQ((char)64, buf.readChar());
}

TEST(MemoryBank, succ_readWriteInt001)
{
    MemoryBank buf(5);
    buf.append((int)555);
    EXPECT_EQ((int)555, buf.readInt());
}

TEST(MemoryBank, succ_readWriteShort001)
{
    MemoryBank buf(5);
    buf.append((short)-555);
    EXPECT_EQ((short)-555, buf.readShort());
}

TEST(MemoryBank, succ_readWriteStringWithLength001)
{
    std::string message("This is a test message."); // 23文字
    MemoryBank buf(5);
    buf.appendStringWithLength(message);
    EXPECT_EQ(23 + sizeof(int), buf.getUsingSize());
    EXPECT_EQ(buf.readInt(), 23);   // 文字列の長さが記録されている
    buf.setCurPosToHead();  // 読み込み位置を先頭に戻す
    EXPECT_EQ("This is a test message.", buf.readStringWithLength());
}

TEST(MemoryBank, succ_readWriteStringWithLength002)
{
  MemoryBank buf(5);
  buf.appendStringWithLength("AB"s);
  buf.appendStringWithLength("CDE"s);
  EXPECT_EQ("AB"s, buf.readStringWithLength());
  EXPECT_EQ("CDE"s, buf.readStringWithLength());
}

// 各種 append と read が正しく動作すること
TEST(MemoryBank, succ001)
{
  MemoryBank buf(5);

  std::string name("Hogehoge!");
  std::string comp("ABCD");
  buf.append((char)10);                  // char
  buf.append((short)123);                // short
  buf.append((int)456);                  // int
  buf.append((long)1111);                // long
  buf.append((float)1.25f);              // float
  buf.append((double)125.75);            // double
  buf.append((bool)false);               // bool (false)
  buf.append((bool)true);                // bool (true)
  buf.appendStringWithLength(name);      // std::string
  buf.appendCharArrayWithLength("ABCD"); // char[]

  EXPECT_EQ((char)10, buf.readChar());
  EXPECT_EQ((short)123, buf.readShort());
  EXPECT_EQ((int)456, buf.readInt());
  EXPECT_EQ((long)1111, buf.readLong());
  EXPECT_EQ((float)1.25f, buf.readFloat());
  EXPECT_EQ((double)125.75, buf.readDouble());
  EXPECT_EQ((bool)false, buf.readBool());
  EXPECT_EQ((bool)true, buf.readBool());
  EXPECT_EQ(name, buf.readStringWithLength());
  EXPECT_EQ(comp, buf.readStringWithLength());
}

// append した結果を汎用的な read で読み取れること
TEST(MemoryBank, succ002)
{
  MemoryBank buf(5);

  // 書きこみ
  buf.append(5);
  buf.append(6);
  buf.append(7);

  // 読み込み
  int a, b, c = 0;
  buf.read(a);
  buf.read(b);
  buf.read(c);
  EXPECT_EQ(5, a);
  EXPECT_EQ(6, b);
  EXPECT_EQ(7, c);
}

// append した結果、メモリ使用領域が拡大されていくこと
TEST(MemoryBank, succ003)
{
  MemoryBank buf(5);

  EXPECT_EQ(0, buf.getAllocMemorySize()); // まだメモリ利用なし
  EXPECT_EQ(0, buf.getUsingSize());       // まだメモリ利用なし

  // 1 バイト書きこみ
  buf.append('A');
  EXPECT_EQ(5, buf.getAllocMemorySize()); // 1スロットのメモリ領域確保
  EXPECT_EQ(1, buf.getUsingSize());       // 1バイト格納

  // 100 バイト書きこみ
  for (int i = 0; i < 100; i++)
  {
    buf.append((char)i);
  }
  EXPECT_EQ(105, buf.getAllocMemorySize()); // 21 スロットのメモリ領域確保
  EXPECT_EQ(101, buf.getUsingSize());       // 101 バイト格納
}

// get / set でカーソル位置関係なくデータの読み込み、上書きができること
TEST(MemoryBank, succ004)
{
  MemoryBank buf(5);
  buf.append('A');

  buf.append('B');
  buf.append('C');
  buf.append('D');
  // get できること。get はカーソル位置関係ないこと
  EXPECT_EQ('B', buf.get(1));
  EXPECT_EQ('A', buf.get(0));

  // set できること。
  buf.set(0, 'E');
  buf.set(1, 'F');

  // get でデータが更新されたことを確認
  EXPECT_EQ('E', buf.get(0));
  EXPECT_EQ('F', buf.get(1));

  // readChar でカーソル操作で読み込み確認
  EXPECT_EQ('E', buf.readChar());
  EXPECT_EQ('F', buf.readChar());
  EXPECT_EQ('C', buf.readChar());
  EXPECT_EQ('D', buf.readChar());
}

// カーソル位置を初期化（先頭）に設定できること
TEST(MemoryBank, succ005)
{
  MemoryBank buf(5);
  buf.append('A');
  buf.append('B');

  // readChar でカーソル操作で読み込み確認
  EXPECT_EQ('A', buf.readChar());
  EXPECT_EQ('B', buf.readChar());

  // カーソル位置を初期か
  buf.setCurPosToHead();

  // readChar でカーソル操作で読み込み確認
  EXPECT_EQ('A', buf.readChar());
  EXPECT_EQ('B', buf.readChar());
}

// appendCharArray で想定通りのバイト列データが追加されること
TEST(MemoryBank, appendCharArray_succ001)
{
  MemoryBank buf;
  Crc32 crc;
  buf.appendCharArray("Test");
  buf.appendCharArray("001");
  buf.appendString("abcd");
  buf.appendString("EFGH");

  char result[32] = {0};
  for (int i = 0; i < 15; i++)
  {
    result[i] = buf.get(i);
  }
  std::string resultStr(result);
  EXPECT_EQ(resultStr, "Test001abcdEFGH");
}

// MemoryBank の crc32 結果と Crc32 クラスの結果を突合する
TEST(MemoryBank, crc_largedata)
{
  MemoryBank buf;
  Crc32 crc;
  for (int i = 0; i < 1024 * 10; i++)
  {
    unsigned char data = (unsigned char)(i % 255);
    buf.appendByte(data);
    crc.calcUpdate(data);
  }
  EXPECT_EQ(485967120, buf.calcCrc32());
  EXPECT_EQ(485967120, crc.getHash());
}

// MemoryBank で
TEST(MemoryBank, readStringToEnd_001)
{
  MemoryBank buf;
  buf.appendString("This is a test."); // 書き込み
  buf.appendString("ABCD.");           // 書き込み

  EXPECT_EQ("This is a test.ABCD.", buf.readStringToEnd()); // 読み込み結果が書き込みと一致
}
