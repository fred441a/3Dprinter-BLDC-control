#include "src/Serial_Debug_printer.h"

void setup() {
  SerialPrinter printer = new SerialPrinter(115200, DEBUG);
  IOManager io = new IOManager();

  Globals::CreateGlobals(printer,io);
  
}

void loop() {}
