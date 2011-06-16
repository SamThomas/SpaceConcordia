#ifndef _SS_THREAD_
#define _SS_THREAD_

#include <QThread>

extern "C"{
#include "core.h"
}


class SimSatThread : public QThread
{
 public:
  SimSatThread();
  void run();
  void setRunState(bool running);
 private:
  long long cycles;
  bool isRunning;
};

#endif
