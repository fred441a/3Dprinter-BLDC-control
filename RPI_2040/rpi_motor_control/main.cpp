#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
const double wanted_ws = 9;
const float Kaw = 3.2;

int main() {
  stdio_init_all();
  Encoder *encoder = new Encoder(19);
  step_response(encoder);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(0.048, 3.2, 0, Kaw);

  while (true) {
      printf("BOOBOO");
    float ws = encoder->get_ws();
    float voltage = pid->voltage(encoder->get_ws(), wanted_ws);
    printf("with AW: voltage:%f,WS:%f\n", voltage, ws);
    motor->set_voltage(voltage);
    // sleep_ms(1000);
  }

  return 0;
}
