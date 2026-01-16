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
  const int enable_pin = 21 ;
  const int direction_pin = 20;
  const float radprstep = 0.010149730730815501;

public:
  Step(std::queue<float> *recieved_ws, std::queue<bool> *alarm_queue)
      : recieved_ws(recieved_ws), alarm_queue(alarm_queue) {};

  void loop() {
    for (;;) {
      if (!get_gpio(enable_pin)) {
        recieved_ws->push(0);
        continue;
      }

      static bool old = false;
      bool NEW = gpio_get(step_pin);
      if (NEW && !old) {
        static int count = 0;
        static float rads = 0;
        static long int last_step_time = get_absolute_time();
        long int step_time = get_absolute_time();
        rads += radprstep * (step_time - last_step_time);

        last_step_time = step_time;

        count++;

        if (count >= 100) {
          printf("Rads: %f\n", rads / 100.0);
          if (get_gpio(direction_pin)) {
            rads = rads * -1;
          }
          recieved_ws->push(rads / 100.0);
          count = 0;
          rads = 0;
        }
      }
      old = NEW;
    }
  }
};
