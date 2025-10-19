#include "Process.h"
#pragma once
class RepeatingProc : public Process {
private:
  int periode; //!< How often does the process need to run
public:
  /**
   * @param func pointer to function the process runs
   * @param periode How often the process runs
   */
  RepeatingProc(void *func, int periode);
};
