#include "FileApiConvert.hpp"

using namespace nl;

std::string FileApiConvert::wrappedGetCwd()
{
#if defined(_WIN32) && defined(__GNUC__)
  // Windows環境 Mingw + GCC 環境向け
  // std::cout << "[DEBUG] FileApiConvert::wrappedGetCwd() : Windows + Mingw GetCWD" << std::endl;
  char path[1024 * 16];
  getcwd(path, 1024 * 16);
  return std::string(path);
#elif defined(_WIN32) && defined(_MSC_VER)
  // Windows 環境 + VisutlStudio
  // std::cout << "[DEBUG] FileApiConvert::wrappedGetCwd() : Windows 環境用 GetCWD" << std::endl;
    char buffer[1024 * 16] = {0};
  return std::string(_getcwd(buffer, 1024*16));
#elif defined(__GNUC__)

  // GCC 環境向け
  // std::cout << "[DEBUG] FileApiConvert::wrappedGetCwd() : Linux 環境用 GetCWD" << std::endl;
  char path[1024 * 16];
  getcwd(path, 1024 * 16);
  return std::string(path);
#endif
};

bool FileApiConvert::wrappedMkDir(std::string dirname)
{
#if defined(_WIN32) && defined(__GNUC__)
  // Windows 上の Mingw で gcc コンパイルした場合
  if (mkdir(dirname.c_str()) == 0)
  {
    // ディレクトリ作成成功
    return true;
  }
  errno = 0;
  return false;
#elif defined(_WIN32) && defined(_MSC_VER)
  // MS VC でコンパイルした場合
  // _mkdir は direct.h に含まれる
  if (_mkdir(dirname.c_str()) == 0)
  {
    // ディレクトリ作成成功
    return true;
  }
  errno = 0;
  return false;
#else
  // mkdir は <sys/stat.h> に含まれる
  if (mkdir(dirname.c_str(), 0777) == 0)
  {
    // Directory 作成成功
    return true;
  }
  errno = 0;
  return false;
#endif
}

bool FileApiConvert::wrappedRmDir(std::string dirname)
{
  if (_rmdir(dirname.c_str()) == 0)
  {
    // ディレクトリ削除成功
    return true;
  }
  return false;
}

bool FileApiConvert::wrappedRmFile(std::string filename)
{
#if defined(_WIN32) && defined(__GNUC__)
  if (_unlink(filename.c_str()))
  {
    return false;
  }
#elif defined(_WIN32) && defined(_MSC_VER)
  if (_unlink(filename.c_str()))
  {
    return false;
  }
#else
  if (unlink(filename.c_str()))
  {
    return false;
  }
#endif
  return true;
}
