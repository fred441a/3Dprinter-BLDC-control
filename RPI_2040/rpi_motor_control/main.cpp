#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"

const uint gpio = 17;
const double wanted_ws = 9.5;

int main() {
  stdio_init_all();
  Motor *motor = new Motor(16, 0.3599);
  Encoder *encoder = new Encoder(17);
  PID *pid = new PID(5.5, 1005, 0);

  while (true) {
    double voltage = pid->voltage(encoder->get_ws(), wanted_ws);
    motor->set_voltage(voltage);
  }
  return 0;
}
