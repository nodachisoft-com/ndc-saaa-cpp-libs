
// clude "../../common_header/pch.h"
//#include "pch.h"
#include "../../pch.h"
#include "Crc32.hpp"
#include <stdio.h>

using namespace nl;

Crc32::Crc32()
{
    hash = 0xffffffff;
    generate_table();
}

void Crc32::generate_table()
{
    // CRC-32-IEEE 802.3 規格に準ずる
    const unsigned long poly = 0xedb88320;
    for (int i = 0; i < 256; i++)
    {
        unsigned long n = i;
        for (int j = 0; j < 8; j++)
        {
            unsigned long t = n & 1;
            n >>= 1;
            if (t)
            {
                n ^= poly;
            }
        }
        table[i] = n;
    }
}

unsigned long Crc32::calcUpdateBytes(void* d, size_t len)
{
    unsigned char* c = (unsigned char*)d;
    for (int i = 0; i < len; i++)
    {
        hash = (hash >> 8) ^ table[(hash & 0xff) ^ c[i]];
    }
    return hash ^ 0xffffffff;
}

unsigned long Crc32::calcUpdateBytes(const std::string msg)
{
    return calcUpdateBytes((void*)msg.c_str(), msg.size());
}

void Crc32::calcUpdate(unsigned char c)
{
    hash = (hash >> 8) ^ table[(hash & 0xff) ^ c];
}

unsigned long Crc32::getHash()
{
    return hash ^ 0xffffffff;
}

