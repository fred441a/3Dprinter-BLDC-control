#include "Serial_Debug_printer.h"

SerialPrinter::SerialPrinter(int baudrate) {
#ifdef debug
  printQueue = xQueueCreate(500, 280);
  Serial.begin(baudRate);
#endif
}

void SerialPrinter::print(std::string str) {
#ifdef debug
  if (str.size() > 280) {
    this->print("String to long to print");
    return;
  }
  xQueueSend(this->printQueue, str, 10);
#endif
};

void SerialPrinter::Process() {
#ifdef debug
  int messages = uxQueueMessagesWaiting(this->printQueue);
  for (int i = 0; i < messages; i++) {
    std::string message;
    xQueueReceive(this->printQueue, &messages, 10);
    Serial.print(messages);
  }
#endif
};
