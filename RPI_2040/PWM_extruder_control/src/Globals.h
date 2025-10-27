#include "Debug_Printer.h"
#include "IO_Manager.h"
#include <FreeRTOS.h>
#include <semphr.h>

/**
 * Singleton Globals class used to store print and io functions
 */
class Globals {
private:
  Globals *instanceptr;
  Globals() {}
  DebugPrinter printer;
  IOManager io;

  // Singleton lock
  SemaphoreHandle_t lock;

public:
  // delete copy operator
  void operator=(Globals const &) = delete;
  void CreateGlobals(DebugPrinter printer, IOManager io);
};
