#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
float wanted_ws = 12;
const float T = 0.0105;
int c = 1;

float KP = 0.11;
float KI = 1.42;
float KD = 0;

int main()
{
  stdio_init_all();
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD);
  sleep_ms(5000);
  printf("begin");
  while (true)
  {
    float ws;
    static float voltage_pid = 0;
    printf("%lld,%f,%f \n", get_absolute_time(), ws, voltage_pid);

    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
  }
}
