#include "FileAccessor.hpp"


using namespace nl;

FileAccessor::FileAccessor(std::string _filepath)
{
  filePath = _filepath;
  filesize = 0;
  isOpenFileFlag = false;

  fileStatus = FileStatus::NOT_LOADING;
  progress = 0.0f;

  memory = new MemoryBank();

  std::filesystem::path path(filePath);
  if (std::filesystem::is_directory(path))
  {
    filetype = FileType::DIR;
  }
  else if (std::filesystem::is_regular_file(path))
  {
    filetype = FileType::FILE;
    struct stat st;
    int statResult = stat(_filepath.c_str(), &st);
    if (statResult != 0)
    {
      // 読み取りエラー
      filetype = FileType::FILE_NOT_FOUND;
    } else {
      filesize = st.st_size; // ファイルサイズ取得
    }
  }
  else {
    filetype = FileType::FILE_NOT_FOUND;
  }
}

FileAccessor::~FileAccessor()
{
}

unsigned long FileAccessor::calcMemoryCrc32()
{
  return memory->calcCrc32();
}

bool FileAccessor::readFileSync()
{
  if (filetype == FileType::DIR || filetype == FileType::FILE_NOT_FOUND)
  {
    // ディレクトリ or ファイルが存在しないなら読み取り不可
    return false;
  }

  long READ_BLOCK_LENGTH = 16;
  FILE* fp;
  fp = fopen(filePath.c_str(),"rb");
  //errno_t err = fopen_s(&fp, filePath.c_str(), "rb"); // for windows only
  //if (err != 0) {
  if ( fp == NULL ) {
      // ファイルオープンエラーの処理
      return false;
  }

  // ファイルサイズの取得
  if (fseek(fp, 0, SEEK_END) != 0)
  {
    // fseek エラーの処理
    return false;
  }

  filesize = ftell(fp);

  if (fseek(fp, 0L, SEEK_SET) != 0)
  {
    // fseek エラーの処理
    return false;
  }

  // ファイル全体を格納するメモリを割り当てる
  char *buffer = (char *)malloc(READ_BLOCK_LENGTH);
  if (buffer == NULL)
  {
    // メモリ割り当てエラーの処理
    return false;
  }

  while (true)
  {
    size_t readByteLength = fread(buffer, 1, READ_BLOCK_LENGTH, fp);
    for (size_t c = 0; c < readByteLength; c++)
    {
      memory->appendByte(buffer[c]);
    }
    if (readByteLength < READ_BLOCK_LENGTH)
    {
      // ファイル読み取り終了
      break;
    }
  }
  free(buffer);
  fclose(fp);
  filetype = FileType::FILE;
  fileStatus = FileStatus::AVAILABLE;
  progress = 1.0f;
  return true;
}

bool FileAccessor::writeFileSync()
{
  FILE* fp;
  fp = fopen(filePath.c_str(), "wb");
  //errno_t err = fopen_s(&fp, filePath.c_str(), "wb");
  //if (err != 0) {
  if ( fp == NULL) {
    // ファイルオープンエラーの処理
    return false;
  }

  long size = memory->getUsingSize();

  for (int i = 0; i < size; i++)
  {
    fputc(memory->get(i), fp);
  }
  // std::cout << std::endl;
  fclose(fp);
  filetype = FileType::FILE;
  fileStatus = FileStatus::AVAILABLE;
  progress = 1.0f;
  return true;
}

MemoryBank *FileAccessor::getMemoryBank()
{
  return memory;
}
void FileAccessor::setMemoryBank(MemoryBank *_memory)
{
  if (memory != NULL)
  {
    // 既存のメモリを解放
    delete memory;
  }
  memory = _memory;
}
std::string FileAccessor::getFilePath()
{
  return filePath;
}
void FileAccessor::setFilePath(std::string _filePath)
{
  filePath = std::string(_filePath);
}
long FileAccessor::getFilesize()
{
  return filesize;
}
FileType FileAccessor::getFiletype()
{
  return filetype;
}
bool FileAccessor::isOpenFile()
{
  return isOpenFileFlag;
}
float FileAccessor::getProgress()
{
  return progress;
}
FileStatus FileAccessor::getFileStatus()
{
  return fileStatus;
}

// ================ 以下 static で提供する機能 ====================
// ファイルへ追記
bool FileAccessor::appendStringToFileSync(const std::string filepath, const std::string text)
{

  // 文字列追記処理を行う間、保持される FILE へのハンドラ
  FILE* appendWriteFp;
  appendWriteFp = fopen( filepath.c_str(), "ab");
  //errno_t err = fopen_s(&appendWriteFp, filepath.c_str(), "ab");
  // if (err != 0) {
  if ( appendWriteFp == NULL ) {
    // ファイルオープンエラーの処理
    return false;
  }

  size_t size = text.size();
  for (size_t i = 0; i < size; i++)
  {
    fputc(text[i], appendWriteFp);
  }
  fclose(appendWriteFp);
  return true;
}
