#include "Simulator.h"

Simulator::Simulator()
{
  //we start in the non running state
  simRunning = false;

  //create our satellite object
  satellite = new SimSatThread();

  //create our main window object
  window = new QMainWindow();

  Ui::MainWindow ui;
  ui.setupUi(window);

  //connect the our slots to the signals we want
  QObject::connect(ui.btn_Start, SIGNAL(clicked()), this, SLOT(simStart()));
  QObject::connect(ui.btn_Stop, SIGNAL(clicked()), this, SLOT(simStop()));

  QObject::connect(this, SIGNAL(appendConsole(const QString &)), ui.txt_SimConsole, SLOT(append(const QString)));

  window->show();
}

void Simulator::_simprintf(char * fmt, ...)
{
  va_list args;
  va_start(args,fmt);

  //create the qstring
  QString qs;
  qs.vsprintf(fmt, args);
  va_end(args);

  //send the signal
  emit appendConsole(qs);
}


void Simulator::simStart()
{
  satellite->setRunState(true);

  if(!satellite->isRunning())
    satellite->start();

}

void Simulator::simStop()
{
  satellite->setRunState(false);
}


