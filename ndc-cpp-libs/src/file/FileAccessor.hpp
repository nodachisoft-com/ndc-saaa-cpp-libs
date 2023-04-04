/**
 * @brief 個々のファイルの読み書き・ディレクトリを含むメタ情報の透過的管理の機能を提供する
 */
#pragma once
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include "../memory_bank/index.hpp"

namespace nl
{
  /// @brief ファイルの状況を定義
  enum class FileStatus
  {
    READING,    /// @brief ファイルを I/O 読み込み中のステータス。外部からのメモリへのアクセスは原則不可
    AVAILABLE,  /// @brief ファイル内容の読み取り可能
    WRITTINGS,  /// @brief ファイルを I/O 書きこみ中のステータス。外部からのメモリへの書きこみは原則不可
    NOT_LOADING /// @brief ファイル読み取り未済。新規作成直後
  };

  /// @brief ファイルオブジェクトの種類
  enum class FileType
  {
    FILE_NOT_FOUND, /// @brief  対象が存在しない
    DIR,            /// @brief 対象はディレクトリ
    FILE            /// @brief 対処はファイル
  };

  /// @brief 一つのファイルへの読み書きの透過的処理、メタ情報の取得をする機能を提供するクラス
  class FileAccessor
  {
  private:
    /// @brief 読み書き対象のデータ管理先
    MemoryBank *memory;

    /// @brief 対象ファイル / ディレクトリへのパス
    std::string filePath;

    /// @brief 対象ファイルのサイズ。ディレクトリなら 0 が保持される
    long filesize;

    /// @brief ファイル種類（ファイル、ディレクトリ、存在しない）
    FileType filetype;

    /// @brief ファイルポインタを操作中であれば true
    bool isOpenFileFlag;

    /// @brief ファイルの読み書き進行具体を記録
    float progress;

    /// @brief ファイル状況を管理
    FileStatus fileStatus;

  public:
    /// @brief コンストラクタ。対象パスのメタ情報を初期化時に収集する
    /// @param[in] filepath 対象パス
    FileAccessor(std::string filepath);
    ~FileAccessor();

    /// @brief 読み書き管理データ内容の CRC32 を計算して返す
    /// @return CRC32 計算結果
    unsigned long calcMemoryCrc32();

    /// @brief 初期化時 / setFilePath で指定したパスのファイルを同期処理で読み込み
    /// TODO 対象がディレクトリ、読み込み権限不足などで処理失敗した場合は false を返すようにすべき
    /// TODO 非同期処理が走っている場合は false を返すべき
    bool readFileSync();

    /// @brief 初期化時 / setFilePath で指定したパスのファイルに同期処理で上書き
    /// TODO 対象がディレクトリ、書き込み権限不足などで処理失敗した場合は false を返すようにすべき
    /// TODO 非同期処理が走っている場合は false を返すべき
    bool writeFileSync();

    // ====== 以下はアクセサメソッド ======
    /// @brief 読み書き対象データへのポインタを取得
    MemoryBank *getMemoryBank();

    /// @brief 読み書き対象データを上書きする。上書きされるメモリは解放処理を行う。
    /// @param[in] memory 上書きするデータ
    void setMemoryBank(MemoryBank *memory);

    /// @brief 扱うファイル / ディレクトリへの絶対パス / 相対パスを取得する
    /// @return パス
    std::string getFilePath();

    /// @brief 初期化時に取得したパスに存在する、扱うファイル / ディレクトリへの絶対パス / 相対パスを上書きする
    /// @param filePath
    void setFilePath(std::string filePath);

    /// @brief 初期化時に取得したパスに存在するファイルサイズを取得する。ディレクトリなら 0 が返る
    /// @return ファイルサイズ（Byte）
    long getFilesize();

    /// @brief 初期化時に取得したパスに存在するファイル種類（ファイル、ディレクトリ、存在しない）を取得する
    /// @return ファイル種類
    FileType getFiletype();

    /// @brief ファイルポインタが開いている状態かを返す
    /// @return ファイルポインタが開いている状態なら TRUE を返す
    bool isOpenFile();

    /// @brief ファイル読み書きの進行状況を取得する
    /// @return [0.0f , 1.0f] （進捗 0% なら 0.0f、進捗 100% なら 1.0f を返す）
    float getProgress();

    /// @brief ファイルの状況を返す。読み書きの最中か、などを判別可能
    FileStatus getFileStatus();

  public:
    // ============ Static 関数。メモリ操作不要な単体で完結する操作 =========

    /// @brief 初期化時 / setFilePath で指定したパスのファイルに同期処理で文字列を追記（改行なし）
    /// @param[in] text 追記するテキスト
    /// TODO 対象がディレクトリ、書き込み権限不足などで処理失敗した場合は false を返すようにすべき
    /// TODO 非同期処理が走っている場合は false を返すべき
    static bool appendStringToFileSync(const std::string filepath, const std::string text);
  };
};
