#define __FREERTOS 1
#include "Debug_Printer.h"
#include <FreeRTOS.h>
#include <queue.h>

#pragma once
/**
 * DebugPrinter instance for ARDUINO serial debugging with FreeRTOS
 */
class SerialPrinter : public DebugPrinter {
private:
  static QueueHandle_t printQueue;

public:
  SerialPrinter(int baudRate);
  void print(std::string str);
  void Process();
};
