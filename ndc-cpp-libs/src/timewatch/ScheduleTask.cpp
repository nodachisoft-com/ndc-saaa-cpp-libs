#include "ScheduleTask.hpp"

using namespace nl;

/// @brief コンストラクタで初期化を行う
ScheduleTask::ScheduleTask(const int _taskcode, const int64_t _relTimeMs)
    : taskcode(_taskcode), relTimeMs(_relTimeMs), progressMs(0L), _isCompleted(false)
{
}

int64_t ScheduleTask::addProgressMs(int64_t addDeltaTimeMs)
{
  int64_t leftTime = 0;
  if (addDeltaTimeMs + progressMs > relTimeMs)
  {
    leftTime = addDeltaTimeMs + progressMs - relTimeMs;
    progressMs = relTimeMs;
    // タスクの時間が超過したため、完了フラグをたてる
    _isCompleted = true;
  }
  else
  {
    progressMs += addDeltaTimeMs;
    leftTime = 0;
  }
  return leftTime;
}

float ScheduleTask::getTaskProgress()
{
  return progressMs / (float)relTimeMs;
}

int ScheduleTask::getTaskcode()
{
  return taskcode;
}

int64_t ScheduleTask::getRelTimeMs()
{
  return relTimeMs;
}

int64_t ScheduleTask::getProgressMs()
{
  return progressMs;
}

bool ScheduleTask::isCompleted()
{
  return _isCompleted;
}
