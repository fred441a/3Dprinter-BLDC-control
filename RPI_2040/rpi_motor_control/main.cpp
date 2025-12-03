#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
float wanted_ws = 9;
const float T = 0.01025;
int c = 1;

float KP = 0.0901;
float KI = 1.4;
float KD = 0;

int main()
{
  stdio_init_all();
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD);

  while (true)
  {
    float ws;
    ws = encoder->get_ws();
    float voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
    printf("speed:%f , voltagePID:%f\n", ws, voltage_pid);

  }
}
