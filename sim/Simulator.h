#ifndef _SIMU_H_
#define _SIMU_H_

#include <stdarg.h>

#include <QObject>
#include <QMainWindow>

#include "ui_SimSat.h"
#include "SimSatThread.h"

class Simulator : public QObject
{
  Q_OBJECT

    public:
  
  //Construcor
  Simulator();

  void _simprintf(char * str, ...);

  public slots:

  //recieve this to start the simulation
  void simStart();

  //recieve this to stop the simulation
  void simStop();

 signals:
  
  void appendConsole(const QString &txt);


 private:
  //state of the simulator
  bool simRunning;

  //UI master object
  QMainWindow *window;

  SimSatThread *satellite;

};

#endif
