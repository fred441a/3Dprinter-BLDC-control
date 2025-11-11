#include "pico/stdlib.h"
#include "pico/time.h"
#include <math.h>
#include <sys/types.h>

#include <cstdio>

/*
class Encoder {
private:
*/
double angular_vel, duty, period;
uint pin;
absolute_time_t time;

void rising() { time = get_absolute_time(); }

void falling() {
  static double degree, old_degree = 0;
  double AM = 0;
  int64_t high_time = abs(absolute_time_diff_us(time, get_absolute_time()));
  duty = (double)high_time / (float)(period * pow(10, 6));
  old_degree = degree;
  degree = duty * 374.609 + 7.49219;

  if (degree > old_degree && abs(degree - old_degree) > 8) {
    AM = (360 - abs(degree - old_degree))/period;
  } else {
    AM = abs(old_degree - degree)/period;
  }

  printf("%lli,%lf,%lf,%lf\n", get_absolute_time(), duty, degree, AM);
}

void irq_callback(uint pin, uint32_t event) {
  switch (event) {
  case GPIO_IRQ_EDGE_RISE:
    rising();
    break;
  case GPIO_IRQ_EDGE_FALL:
    falling();
    break;
  }
}

void init(int pin, int freq) {
  gpio_init(pin);
  period = 1.0 / freq;
  gpio_set_irq_enabled_with_callback(
      pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, irq_callback);
}

float get_degree() {
  // actual min max PWM 0.98-0.02
  return duty * 374.609 + 7.49219;
};

/*
public:
  Encoder() : pin(17), period(1.0 / 905) { init(this); };
  Encoder(uint pin, uint freq) : pin(pin), period(1.0 / freq) { init(this); };
  float get_degree() { return duty * 379.346 + 7.966; };
};
*/
