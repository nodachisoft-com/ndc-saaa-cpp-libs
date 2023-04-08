#pragma once
#include <iostream>
#include "CmdBase.hpp"


namespace nl
{
  class ExitForceCmd : public CmdBase
  {
  public:
    ExitForceCmd();
    ~ExitForceCmd();
    void appExec() override;
  };
}