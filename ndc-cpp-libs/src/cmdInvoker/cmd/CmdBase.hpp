#pragma once

class CmdBase
{
public:
  CmdBase() {}
  virtual void appExec(){};
  void _EntryExec() { appExec(); }
  virtual ~CmdBase() {}
};
