#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
const double wanted_ws = 50;

int main() {
  stdio_init_all();
  Encoder *encoder = new Encoder(19);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(0.05, 1005, 0);

  while (true) {
    float ws = encoder->get_ws();
    float voltage = pid->voltage(encoder->get_ws(), wanted_ws);
    printf("voltage:%f,WS:%f\n", voltage, ws);
    motor->set_voltage(voltage);
    // sleep_ms(1000);
  }

  return 0;
}
