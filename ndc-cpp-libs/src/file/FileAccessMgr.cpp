#include "FileAccessMgr.hpp"

using namespace nl;

FileAccessMgr::FileAccessMgr()
{
}

bool FileAccessMgr::makedir(std::string dirname)
{
  std::error_code ec;
  return std::filesystem::create_directory(dirname, ec);
}

bool FileAccessMgr::removedir(std::string dirname)
{
  std::error_code ec;
  return std::filesystem::remove(dirname, ec);
}

bool FileAccessMgr::removefile(std::string filename)
{
  std::error_code ec;
  return std::filesystem::remove(filename, ec);
}

FileType FileAccessMgr::isExistFileOrDir(std::string filepath)
{
  std::filesystem::path path(filepath);
  if (std::filesystem::is_directory(path))
  {
    return FileType::DIR;
  } else if (std::filesystem::is_regular_file(path))
  {
    return FileType::FILE;
  }
  return FileType::FILE_NOT_FOUND;
}

FileAccessor FileAccessMgr::getFileInfo(std::string filename)
{
  FileAccessor fa(filename);
  return fa;
}

std::string FileAccessMgr::getCurDir()
{
  return std::filesystem::current_path().string();
}

std::vector<FileAccessor> FileAccessMgr::getFilesRecursively(std::string dirPath)
{
    std::vector<FileAccessor> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            FileAccessor fa(entry.path().string());
            files.push_back(fa);
        }
    }
    return files;
}

std::vector<FileAccessor> FileAccessMgr::getDirsRecursively(std::string dirPath)
{
    std::vector<FileAccessor> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
        if (entry.is_directory()) {
            FileAccessor fa(entry.path().string());
            files.push_back(fa);
        }
    }
    return files;
}
