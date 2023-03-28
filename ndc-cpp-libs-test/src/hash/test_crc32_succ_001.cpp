#include "../../pch.h"

using namespace nl;

// CRC32 コードが想定通りであること
TEST(Crc32, case1)
{
    Crc32 crc;
    std::string text("abcd");
    unsigned long result = crc.calcUpdateBytes((void*)text.c_str(), (int)text.length());

    // 計算結果確認
    EXPECT_EQ(0xed82cd11, result);
    EXPECT_EQ(0xed82cd11, crc.getHash());
}

// １文字づつ読み取り方式で、結果が想定通りであること
TEST(Crc32, case2)
{
    Crc32 crc;
    char text[] = "abcd";
    for (int i = 0; i < 4; i++)
    {
        crc.calcUpdate(text[i]);
    }
    EXPECT_EQ(0xed82cd11, crc.getHash());
}

TEST(Crc32, calcUpdate_LargeData_case001)
{
    Crc32 crc;
    for (int set = 0; set < 1000; set++)
    {
        for (unsigned char i = 0; i < 255; i++)
        {
            crc.calcUpdate(i);
        }
    }
    EXPECT_EQ(3952659139, crc.getHash());
}

// 文字列から hash 値（数値）を生成
TEST(Crc32, calcUpdate_calcUpdateBytes_case001)
{
    Crc32 crc;
    unsigned long worldseed = crc.calcUpdateBytes("This is a test world!");

    EXPECT_EQ(2222250284, worldseed);
}
