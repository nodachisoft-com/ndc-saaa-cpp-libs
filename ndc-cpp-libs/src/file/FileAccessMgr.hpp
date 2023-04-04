/**
 * @brief Windows や Linux の OS の差を吸収したファイル操作・情報を取得する機能を提供する
 */
#pragma once
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <filesystem>
#include <iostream>
#include "FileAccessor.hpp"



namespace nl
{

  /// @brief ファイルアクセス用機能を提供するクラス
  class FileAccessMgr
  {
  public:
    /// @brief 処理なし。機能は static にて提供
    FileAccessMgr();

    /// @brief 相対パス / 絶対パスにより指定のディレクトリを作成する。複数の階層のディレクトリを一度に作成することはできない。
    /// @param[in] dirname ディレクトリ名、ディレクトリパス
    /// @return ディレクトリ作成に成功した場合 true を返す
    static bool makedir(std::string dirname);

    /// @brief
    ///    相対パス / 絶対パスにより指定の空ディレクトリを削除する。
    ///    ディレクトリ内にファイルや他のディレクトリが存在する場合は削除に失敗する。
    /// @param[in] dirname ディレクトリ名、ディレクトリパス
    /// @return ディレクトリの削除に成功した場合 true を返す
    static bool removedir(std::string dirname);

    /// @brief 相対パス / 絶対パスにより指定した対象ファイルを削除する
    /// @param[in] filename ファイル名
    /// @return ファイルの削除に成功した場合 true を返す
    static bool removefile(std::string filename);

    /// @brief ファイル、もしくはディレクトリが存在するかを確認する
    /// @param[in] filepath 確認対象のパス
    /// @return 以下の情報を返す
    ///   ファイルなら FileType::FILE
    ///   ディレクトリなら FileType::DIR
    ///   対象が存在しない or 情報取得できないなら FileType::FILE_NOT_FOUND
    static FileType isExistFileOrDir(std::string filepath);

    /// @brief 指定したパスのメタ情報（ファイルサイズ、ファイル種別（Dir / File / Not found）、読み書き用クラスを返す
    /// @param[in] path ファイル/ディレクトリへの絶対パス / 相対パス
    /// @return ファイルのメタ情報・読み書き用クラス
    static FileAccessor getFileInfo(std::string path);

    /// @brief カレントディレクトリの絶対パスを取得する
    /// @return カレントディレクトリの絶対パス（文字列）
    static std::string getCurDir();

    /// @brief 指定のディレクトリ配下のファイル（ディレクトリは除く）を再帰的にすべて FileAccessor 形式で取得する
    /// @param dirPath 再帰的に探索をする対象ディレクトリ
    /// @return ディレクトリ配下のファイル一覧
    static std::vector<FileAccessor> getFilesRecursively(std::string dirPath);

    /// @brief 指定のディレクトリ配下のディレクトリ（ディレクトリは除く）を再帰的にすべて FileAccessor 形式で取得する
    /// @param dirPath 再帰的に探索をする対象ディレクトリ
    /// @return ディレクトリ配下のディレクトリ一覧
    static std::vector<FileAccessor> getDirsRecursively(std::string dirPath);

  };
}
