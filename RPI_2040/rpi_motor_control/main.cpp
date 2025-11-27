#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
float wanted_ws = 11.5;
const float T = 0.01;
int c = 1;

float KP = 0.11;
float KI = 1.4286;
float KD = 0;

int main()
{
  stdio_init_all();
  sleep_ms(1000);
  Encoder *encoder = new Encoder(19);

  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD);

  printf("KP:%f, KI:%f \n", KP, KI);
  printf("absolute time [uS], PID voltage, measured ws, wantede WS \n");
  while (true)
  {
    // printf("raw:%f \n", encoder->get_raw());
    // 7printf("radians:%f \n", encoder->get_radian());

    float ws;
    if (c > 100)
    {
      wanted_ws = 11.5;
    }
    else
    {
      wanted_ws = 0;
      c += c++;
    }
    ws = encoder->get_ws();
    motor->set_voltage(wanted_ws);
    // float voltage = pid->voltage(ws, wanted_ws);
    // float voltage2 = pid->voltageDis(ws, wanted_ws, T);
    printf("%lld,%f\n", get_absolute_time(), ws);
    // printf("%lld,%f,%f, %f\n", get_absolute_time(), voltage2, ws, wanted_ws);
    // motor->set_voltage(voltage2);
  }
}