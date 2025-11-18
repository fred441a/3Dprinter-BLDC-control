#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
const float wanted_ws = 6;

int main() {
  stdio_init_all();
  sleep_ms(5000);
  Encoder *encoder = new Encoder(19);
  // step_response(encoder);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(0.045, 3, 0);

  while (true) {
    float ws = encoder->get_ws();
    float voltage = pid->voltage(encoder->get_ws(), wanted_ws);
    printf("with AW: voltage:%f,WS:%f\n", voltage, ws);
    motor->set_voltage(voltage);
    // sleep_ms(1000);
  }

  return 0;
}
