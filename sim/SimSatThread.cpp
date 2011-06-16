#include "SimSatThread.h"

SimSatThread::SimSatThread()
{
  cycles = 0;
  runState = false;
}

void SimSatThread::run()
{
  while(1){
    if(runState)
      {
	if(!cycles){
	  //first run
	  core_init();
	}
	else{
	  core_loop();
	}
	cycles++;
	printf("\nFinished Cycle %d\n", cycles);
      }
    else{
      //chill
      usleep(500);
    }
  }
}

//may need to deal with synchronization here
void SimSatThread::setRunState(bool running)
{
  //but not now
  runState = running;
}
