#include "ExitForceCmd.hpp"

using namespace nl;

ExitForceCmd::ExitForceCmd()
{
}

void ExitForceCmd::appExec()
{
  // TODO: 各種の必要なデータ保存、リソース解放、スレッド終了など
  std::cout << "Exit App." << std::endl;
}

ExitForceCmd::~ExitForceCmd()
{
}
