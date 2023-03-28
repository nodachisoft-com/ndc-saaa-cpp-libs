/**
 * @file MemoryBank.hpp
 * @brief 利用サイズにあわせて自動的にメモリ領域を拡張するバイト列データを扱う機能を提供します。
 */
#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <sstream>

/// @brief 動的に拡張可能なバイト列を管理することができるクラス
/// @details
///  利用サイズにあわせて自動的にメモリ領域を拡張するバイト列データを扱うクラスです。
///  メモリは一定サイズで拡張されていき、メモリスロットとして配列で参照を管理します。
///  バイト列の末尾に数値型や文字列型、float、double などの基本的な型をバイト列として
///  追記することが可能です。
///  追加されたバイト列は読み取り位置を指定してデシリアライズ可能です。
class MemoryBank
{
  /// デフォルトでの 1 つのメモリスロット当たりの領域は 1 [MB]
  const int DEFAULT_MEMORY_BLOCK_SIZE = 1024 * 1024;

  /// 1 つのメモリスロットに確保するメモリサイズを規定する。
  /// 初期化時にのみ設定可能。 1[KB] ～ 8[MB] 程度を推奨。
  /// 必要に応じて、クラス内部でこの単位でメモリ領域が確保され拡張される。
  /// メモリ確保状況はクラス無いで隠蔽される
  int memoryBlockSize;

  /// バイト列にアクセス可能な最大の要素番号が記録される。
  /// この要素を含む要素番号まで get() などでアクセス可能。
  /// 4バイト(32bit) int型で 2GByte までのサイズを扱える
  int endPos = -1;

  /// バイト列から read 系 関数で読み取るカーソル位置を定義。
  /// 0 ならバイト列の先頭に位置している状態を表す。
  int curPos = 0;

  /// 保持するバイト列が格納される各メモリスロットへの参照を持つメモリスロット配列
  std::vector<unsigned char *> memory;

public:
  /// @brief スケーラブルなバイト列操作用のクラスを初期化する
  MemoryBank();

  /// @brief スケーラブルなバイト列操作用のクラスを初期化する
  /// @param[in] memoryBlockSize クラス内部のメモリ確保の単位(byte）を指定する
  MemoryBank(const int memoryBlockSize);

  /// @brief バイト列の保持で確保した全てのメモリ領域を開放する
  ~MemoryBank();

  /// @brief バイト列の n 番目の値を取得する
  /// @param[in] index 取得するバイト列の要素番号
  /// @return 取得したバイトデータ。
  /// @exception OutOfRangeAccessException - 保持するバイト列の範囲外にアクセス
  unsigned char get(const int index);

  /// @brief バイト列の n 番目の値を上書きする
  /// @param[in] index 上書きするバイト列の要素番号
  /// @param[in] value 上書きするバイトデータ
  /// @exception OutOfRangeAccessException - 保持するバイト列の範囲外にアクセス
  void set(const int index, const unsigned char value);

  /// @brief バイト列の末尾に指定した char を追記する
  /// @param[in] value 追記するバイトデータ
  /// @return Builder パターン実現のための Instance 本体への参照
  MemoryBank *appendByte(const unsigned char value);

  /// @brief バイト列の末尾に指定した型のデータを追記する
  /// @param[in] value 追記するバイトデータ。Primitive 型のみで参照型は不可。
  /// @return Builder パターン実現のための Instance 本体への参照
  template <typename X>
  MemoryBank *append(const X value);

  /// @brief バイト列の末尾に std::string 型の、文字列の長さ＋文字列データ本体を追記する
  /// @param[in] value 追記する std::string データへの参照
  /// @return Builder パターン実現のための Instance 本体への参照
  MemoryBank *appendStringWithLength(const std::string &value);

  /// @brief
  ///  バイト列の末尾に char[] 型の、文字列の長さ＋文字列データ本体を追記する
  ///  追記した文字列データは readString() で読み取り可能
  /// @param[in] value 追記する char[] データへの参照
  /// @return Builder パターン実現のための Instance 本体への参照
  MemoryBank *appendCharArrayWithLength(const char value[]);

  /// @brief バイト列の末尾に char[] 型を追加する。文字列の NULL は追加されないことに注意。
  /// @param value 追加する文字列
  /// @return Builder パターン実現のための Instance 本体への参照
  MemoryBank *appendCharArray(const char value[]);

  /// @brief バイト列の末尾に String データの文字列を追加する。文字列の NULL は追加されないことに注意。
  /// @param value 追加する文字列
  /// @return Builder パターン実現のための Instance 本体への参照
  MemoryBank *appendString(const std::string value);

  /// @brief 読み取りカーソル位置から char データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った char データ
  char readChar();

  /// @brief 読み取りカーソル位置から short データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った short データ
  short readShort();

  /// @brief 読み取りカーソル位置から int データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った int データ
  int readInt();

  /// @brief 読み取りカーソル位置から long データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った long データ
  long readLong();

  /// @brief 読み取りカーソル位置から float データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った float データ
  float readFloat();

  /// @brief 読み取りカーソル位置から double データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った double データ
  double readDouble();

  /// @brief 読み取りカーソル位置から bool データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った bool データ
  bool readBool();

  /// @brief 読み取りカーソル位置から std::string データを読み取りカーソルを進める
  /// @return カーソル位置から読み取った std::string データ
  std::string readStringWithLength();

  /// @brief MemoryBank に読み込んだファイルデータを全て文字列として読み取り String 型で返す
  /// @return MemoryBank の最後までを読み取った文字列データ
  std::string readStringToEnd();

  /// @brief 読み取りカーソル位置から指定した型データを読み取り、カーソルを進める
  /// @param[out] value データを読み取り上書きする先の変数
  template <typename X>
  void read(X &value);

  /// @brief 読み取りカーソル位置をバッファの先頭にセット
  void setCurPosToHead();

  /// @brief
  ///  バイト列を扱うことが可能な領域として、既に確保しているメモリ使用量をバイト数で取得します。
  ///  「使用量 = DEFAULT_MEMORY_BLOCK_SIZE x 確保済みメモリスロット数」で計算されます。
  /// @return バッファのメモリ使用量（バイト数）
  int getAllocMemorySize();

  /// @brief 管理しているバイト列の長さを取得します。
  /// @return バイト列の長さ。
  int getUsingSize();

  /// @brief CRC32 の計算を行う
  /// @return CRC32 計算結果を返す
  unsigned long calcCrc32();

  /// @brief クラス内で使用しているメモリスロット数、メモリ容量などのデバッグ情報を標準出力します
  void debug();

private:
  /// @brief 新しくメモリスロットにメモリを割り当て、確保した領域をすべて \0 で初期化します。
  /// @return 成功なら TRUE、メモリ確保に失敗した場合 FALSE を返します。
  bool useNewMemorySlot();
};
