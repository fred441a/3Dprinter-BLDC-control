#include "PWM_Measure.h"

PWMMeasure::PWMMeasure(int pin, int freq, int resolution) {
  this->pin = pin;
  this->freq = freq;
  this->resolution = resolution;
  this->Process = new RepeatingProc(&this->Process, 1 / (freq * resolution));
  this->LMmutex = xSemaphoreCreateMutex();
}


