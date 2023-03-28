/**
 * @brief ファイル I/O 処理の低レベル API について、 OS の差異を吸収した API として提供する
 */
#pragma once
#include <string>
#include <iostream>

#if defined(_MSC_VER)
// Windows 環境
#include <direct.h> // Microsoft の File API 利用

#elif defined(__GNUC__)
// Linux 環境
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace nl
{
  /// @brief ファイルI/O 処理の API を static 関数としてクラスでまとめて提供
  class FileApiConvert
  {
  public:
    /// @brief カレントディレクトリの絶対パスを取得
    /// @return 絶対パス（文字列）
    static std::string wrappedGetCwd();

    /// @brief
    ///   ディレクトリを作成する
    ///   再帰的に深い階層のディレクトリの作成はしない。現在存在するディレクトリ配下に一つディレクトリを作成する
    /// @param[in] dirpath 作成するディレクトリのパス
    /// @return 成功した場合に true を返す
    static bool wrappedMkDir(std::string dirpath);

    /// @brief 一つの空ディレクトリを削除する。
    /// @param[in] dirpath 削除するディレクトリのパス
    /// @return 成功した場合に true を返す
    static bool wrappedRmDir(std::string dirpath);

    /// @brief 一つのファイルを削除する。
    /// @param[in] filepath 削除するファイルのパス
    /// @return 成功した場合に true を返す
    static bool wrappedRmFile(std::string filepath);
  };
}
