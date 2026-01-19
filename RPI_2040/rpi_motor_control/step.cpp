#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <math.h>
#include <queue>
#include <stdio.h>

class Step {
private:
  std::queue<float> *recieved_ws;
  std::queue<bool> *alarm_queue;
  const int step_pin = 22;
  const int enable_pin = 21;
  const int direction_pin = 20;
  const float radprstep = 866.8891360771228; // 0.010149730730815501;

public:
  Step(std::queue<float> *recieved_ws, std::queue<bool> *alarm_queue)
      : recieved_ws(recieved_ws), alarm_queue(alarm_queue) {};

  void loop() {
    for (;;) {
      static bool disable_sent = false;
      if (gpio_get(enable_pin)) {
        if (!disable_sent) {
          recieved_ws->push(0);
          disable_sent = true;
        }
        continue;
      }

      disable_sent = false;

      static bool old = false;
      bool NEW = gpio_get(step_pin);
      static long int last_step_time = get_absolute_time();
      long int step_time = get_absolute_time();

      static bool timer_triggered = false;

      if (NEW && !old) {
        timer_triggered = false;
        static int count = 0;
        static float rads = 0;
        rads += radprstep * 1 / (step_time - last_step_time);

        last_step_time = step_time;

        count++;

        if (count >= 100) {
          if (!gpio_get(direction_pin)) {
            rads = 0; // rads * -1;
          }

          // printf("Rads: %f\n", rads / 100.0);
          recieved_ws->push(rads / 100.0);
          count = 0;
          rads = 0;
        }
      }
      old = NEW;

      if ((step_time - last_step_time) > 400 & !timer_triggered) {
        recieved_ws->push(0);
        timer_triggered = true;
      }
    }
  }
};
