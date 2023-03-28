#include "FileAccessMgr.hpp"

using namespace nl;

FileAccessMgr::FileAccessMgr()
{
}

bool FileAccessMgr::makedir(std::string dirname)
{
  return FileApiConvert::wrappedMkDir(dirname);
}

bool FileAccessMgr::removedir(std::string filename)
{
  return FileApiConvert::wrappedRmDir(filename);
}

bool FileAccessMgr::removefile(std::string filename)
{
  return FileApiConvert::wrappedRmFile(filename);
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
  return FileApiConvert::wrappedGetCwd();
}

std::vector<FileAccessor> FileAccessMgr::getFilesRecursively(std::string dirPath)
{
  //std::vector<FileAccessor> result{};
  //return _getInnerFilesRecursively(dirPath, result);
    std::vector<FileAccessor> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            //files.push_back(entry.path().string());
            FileAccessor fa(entry.path().string());
            files.push_back(fa);
        }
    }
    return files;
}

// std::vector<FileAccessor> FileAccessMgr::_getInnerFilesRecursively(std::string dirPath, std::vector<FileAccessor> &filelist)
/*
std::vector<FileAccessor> FileAccessMgr::_getInnerFilesRecursively(std::string dirPath)
{
    std::vector<FileAccessor> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            //files.push_back(entry.path().string());
            FileAccessor fa(entry.path().string());
            files.push_back(fa);
        }
    }
    return files;
    */
/*
  DIR *dir;
  struct dirent *ent;
  std::string fileOrDirName;
  if ((dir = opendir(dirPath.c_str())) != NULL)
  {
    while ((ent = readdir(dir)) != NULL)
    {
      fileOrDirName = std::string(ent->d_name);
      if (strlen(ent->d_name) == 1 && ent->d_name[0] == '.')
      {
      }
      else if (strlen(ent->d_name) == 2 && ent->d_name[0] == '.' && ent->d_name[1] == '.')
      {
      }
      else
      {
        std::string nextDirPath;
        // 指定された PATH の末尾が 「/」 で終わっているかをチェック
        char tailCharOfPath = dirPath[dirPath.length() - 1];
        if (tailCharOfPath == '/')
        {
          nextDirPath = dirPath + fileOrDirName;
        }
        else
        {
          nextDirPath = dirPath + "/" + fileOrDirName;
        }

        FileAccessor fa(nextDirPath);
        if (fa.getFiletype() == FileType::DIR)
        {
          // printf("DIR:: %s\n", nextDirPath.c_str());
          filelist = _getInnerFilesRecursively(nextDirPath, filelist);
        }
        else if (fa.getFiletype() == FileType::FILE)
        {
          filelist.push_back(fa);
          // printf("FILE:: %s\n", nextDirPath.c_str());
        }
        else
        {
          // printf("Unknown:: %s\n", nextDirPath.c_str());
        }
      }
    }
    closedir(dir);
  }
  else
  {

    perror("");
    return filelist;
  }
  */
  // return filelist;
// }

std::vector<FileAccessor> FileAccessMgr::getDirsRecursively(std::string dirPath)
{
  //std::vector<FileAccessor> result{};
  //return _getInnerDirsRecursively(dirPath, result);
    std::vector<FileAccessor> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
        if (entry.is_directory()) {
            //files.push_back(entry.path().string());
            FileAccessor fa(entry.path().string());
            files.push_back(fa);
        }
    }
    return files;
}

/*
std::vector<FileAccessor> FileAccessMgr::_getInnerDirsRecursively(std::string dirPath, std::vector<FileAccessor> &dirlist)
{

  DIR *dir;
  struct dirent *ent;
  std::string fileOrDirName;
  if ((dir = opendir(dirPath.c_str())) != NULL)
  {
    while ((ent = readdir(dir)) != NULL)
    {
      // std::cout << "START LOOP" << std::endl;
      fileOrDirName = std::string(ent->d_name);
      // std::cout << "OBJECT:" << fileOrDirName << std::endl;
      if (strlen(ent->d_name) == 1 && ent->d_name[0] == '.')
      {
      }
      else if (strlen(ent->d_name) == 2 && ent->d_name[0] == '.' && ent->d_name[1] == '.')
      {
      }
      else
      {
        std::string nextDirPath;
        // 指定された PATH の末尾が 「/」 で終わっているかをチェック
        const char *dirPathChars = dirPath.c_str();
        int dirPathLen = strlen(dirPathChars);
        // printf("DIRPATH_LEN=%d\n", dirPathLen);
        char tailCharOfPath = dirPathChars[dirPathLen - 1];
        // for (int i = 0; i < dirPathLen; i++)
        //  {
        //   printf("%c,", dirPathChars[i]);
        // }

        // char tailCharOfPath = dirPath[dirPath.length() - 1];
        // printf("TAIL=%c\n", tailCharOfPath);
        if (tailCharOfPath == '/')
        {
          nextDirPath = dirPath + fileOrDirName;
        }
        else
        {
          nextDirPath = dirPath + "/" + fileOrDirName;
        }
        // std::cout << "NEXT_DIR_PATH:" << nextDirPath << std::endl;
        FileAccessor fa(nextDirPath);
        // std::cout << "INIT" << std::endl;
        if (fa.getFiletype() == FileType::DIR)
        {
          // std::cout << "push_back" << std::endl;
          dirlist.push_back(fa);
          dirlist = _getInnerDirsRecursively(nextDirPath, dirlist);
        }
        // std::cout << "IF END" << std::endl;
      }
      // std::cout << "LOOP END" << std::endl;
    }
    // std::cout << "CloseDir" << std ::endl;
    closedir(dir);
  }
  else
  {

    perror("");
    return dirlist;
  }
  return dirlist;
}
*/