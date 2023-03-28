/**
 * CRC32 計算機能を提供する
 */
#pragma once
#include "../../pch.h"
#include <stdint.h>
#include <string>

namespace nl
{
    /// @brief バイト列に対する CRC32 計算機能を提供するクラス
    class Crc32
    {
    private:
        /// @brief CRC-32-IEEE 802.3 規格に準ずる計算テーブルをキャッシュする
        unsigned long table[256];

        /// @brief CRC32 計算結果を記録する
        unsigned long hash;

        /// @brief CRC-32-IEEE 802.3 規格に準ずる計算テーブルをクラス利用じの最初の一度のみ初期化時に算出する
        void generate_table();

    public:
        /// @brief 計算テーブルの初期化を行う。計算はプログラム全体で最初の一度のみ実施される
        Crc32();

        /// @brief バイト列の CRC32 を計算する。今まで計算した CRC32 に加えて計算を行う。
        /// @param[in] byteBlock 追加で計算を行う対象のバイト列
        /// @param length 計算するバイト列の長さ（バイト数）
        /// @return 計算完了時の CRC32 値
        unsigned long calcUpdateBytes(void* byteBlock, size_t length);

        /// @brief 文字列から数値のハッシュ値を取得する
        /// @param msg 文字列
        /// @return CRC32 計算結果
        unsigned long calcUpdateBytes(const std::string msg);

        /// @brief 1 バイトを追加で CRC32 計算する
        /// @param character 1バイトキャラクタ
        void calcUpdate(unsigned char character);

        /// @brief CRC32 計算結果を取得する
        /// @return CRC32 ハッシュ値
        unsigned long getHash();
    };
};
