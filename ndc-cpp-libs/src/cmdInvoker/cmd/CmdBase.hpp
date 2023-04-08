#pragma once

namespace nl
{
  class CmdBase
  {
  public:
    CmdBase() {}
    virtual void appExec(){};
    void _EntryExec() { appExec(); }
    virtual ~CmdBase() {}
  };
}