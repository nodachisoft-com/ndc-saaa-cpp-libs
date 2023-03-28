#pragma once
#include "CmdBase.hpp"
#include <iostream>

class ExitForceCmd : public CmdBase
{
public:
  ExitForceCmd();
  ~ExitForceCmd();
  void appExec() override;
};
