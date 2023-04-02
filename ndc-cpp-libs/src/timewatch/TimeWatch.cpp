#include "TimeWatch.hpp"

using namespace nl;

TimeWatch::TimeWatch() : tasklist({}), appSpeed(1.0f), timeFromStartMs(0L), stopFlag(true)
{
}

void TimeWatch::start()
{
  stopFlag = false;
}

void TimeWatch::stop()
{
  stopFlag = true;
}

// void TimeWatch::removeCompletedTasks()
//{
// }

bool TimeWatch::isStart()
{
  return !stopFlag;
}

void TimeWatch::removeWholeTasks()
{
  tasklist.clear();
}

void TimeWatch::setAppSpeed(const float speed)
{
  appSpeed = speed;
}

const float TimeWatch::getAppSpeed()
{
  return appSpeed;
}

void TimeWatch::addRealTime(const int64_t deltaTimeMs)
{
  if (stopFlag)
  {
    return;
  }
  int deltaTimePlus = (int)(deltaTimeMs * appSpeed);
  timeFromStartMs += deltaTimePlus;
  int64_t leftTimeMs = deltaTimePlus;
  int size = (int)tasklist.size();
  for (int i = 0; i < size; i++)
  {
    // ScheduleTask eachTask = tasklist[i];
    leftTimeMs = tasklist[i].addProgressMs(leftTimeMs);
    if (leftTimeMs == 0)
    {
      // 時間がタスクに使われた
      break;
    }
  }
}
void TimeWatch::pushTask(ScheduleTask task)
{

  if (task.getTaskcode() < 0)
  {
    std::string message;
    message += "task code must be more than 0. taskcode=[";
    message += task.getTaskcode();
    message += "]";

    // タスクコードが 0 未満であり想定外
    throw ArgumentValidatioinException(message);
  }
  tasklist.push_back(task);
}

bool TimeWatch::checkTaskCompleted(const int taskcode)
{
  int size = (int)tasklist.size();
  for (int i = 0; i < size; i++)
  {
    int eachTaskcode = tasklist[i].getTaskcode();
    if (taskcode == eachTaskcode)
    {
      return tasklist[i].isCompleted();
    }
  }
  return false;
}

int TimeWatch::getNowProgressingTaskcode()
{
  int result = -1;
  int size = (int)tasklist.size();
  for (int i = 0; i < size; i++)
  {
    if (!tasklist[i].isCompleted())
    {
      result = tasklist[i].getTaskcode();
      break;
    }
  }
  return result;
}
float TimeWatch::getProcessingTaskProgress()
{
  float result = -1.0f;
  int size = (int)tasklist.size();
  for (int i = 0; i < size; i++)
  {
    if (!tasklist[i].isCompleted())
    {
      result = tasklist[i].getTaskProgress();
      break;
    }
  }
  return result;
}
float TimeWatch::getWholeTasksProgress()
{
  int64_t wholeAmount = 0;
  int64_t progressAmount = 0;

  int size = (int)tasklist.size();
  if (size == 0)
  {
    // 一つもタスクが存在しないなら 0.0f を返す
    return 0.0f;
  }
  for (int i = 0; i < size; i++)
  {
    wholeAmount += tasklist[i].getRelTimeMs();
    progressAmount += tasklist[i].getProgressMs();
  }
  return progressAmount / (float)wholeAmount;
}
int64_t TimeWatch::getTimeFromStartMs()
{
  return timeFromStartMs;
}

int64_t TimeWatch::getNowMs()
{
  // time_t now = time(nullptr);
  // time_t mnow = now * 1000;
  // return (int64_t)mnow;
  uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  return ms;
}

std::string TimeWatch::getTimezone()
{
  auto now = std::chrono::system_clock::now();
  std::time_t time_now_t = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm;
  errno_t error = localtime_s(&now_tm, &time_now_t);
  // std::tm now_tm = *std::localtime(&time_now_t);
  char buf[512];
  std::strftime(buf, 6, "%z", &now_tm);
  return std::string(buf);
}

std::string TimeWatch::getNowMsAsStrIso8601()
{
  auto dt = std::chrono::system_clock::now();
  std::time_t time_now_t = std::chrono::system_clock::to_time_t(dt);
  std::tm now_tm;
  errno_t error = localtime_s(&now_tm,&time_now_t);
  // std::tm now_tm = *std::localtime(&time_now_t);
  char buf[512];
  std::strftime(buf, 512, "%Y-%m-%dT%H:%M:%S%z", &now_tm);
  return std::string(buf);
}

std::string TimeWatch::getNowMsAsStr_hhmmssSSS()
{
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  // std::tm *now_tm = std::localtime(&now_c);
  std::tm now_tm;
  errno_t error = localtime_s(&now_tm, &now_c);

  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%H:%M:%S.") << std::setfill('0') << std::setw(3) << (now.time_since_epoch() % std::chrono::seconds(1)) / std::chrono::milliseconds(1);

  return oss.str();
}
