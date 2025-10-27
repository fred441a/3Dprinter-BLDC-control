#pragma once
#include "Repeating_Process.h"
#define __FREERTOS 1
#include <FreeRTOS.h>
#include <semphr.h>

/**
 * This is a class that measures PWM
 */
class PWMMeasure {
private:
  int pin;
  int freq;
  int resolution;
  // The value of the pwm from 0 to 1;
  int value;
  RepeatingProc *Process;
  void calculate_value();
  // Mutex for latestMeasurement
  SemaphoreHandle_t LMmutex;

public:
  /**
   * @Param pin the pin to measure the PWM on;
   * @Param freq the frequency of the PWM;
   * @Param resolution how many different widths pr period
   */
  PWMMeasure(int pin, int freq, int resolution);

  int GetDegree();
  int GetRadian();
};
