#include "SimSatThread.h"

SimSatThread::SimSatThread()
{
  cycles = 0;
  isRunning = false;
}

void SimSatThread::run()
{

  if(isRunning)
    {
      if(!cycles){
	//first run
	core_init();
      }
      else{
	core_loop();
      }
      cycles++;
    }
  else{
    //chill
    usleep(100);
  }
}
