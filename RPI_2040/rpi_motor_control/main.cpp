#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
float wanted_ws = 9.5;
const float T = 0.010;
float slow_rise = 0;

float ws;
static float voltage_pid;

float KP = 0.13;
float KI = 1.428;
float KD = 0;

// slow start function:
bool slowStart(Encoder *encoder, Motor *motor, PID *pid, float wanted_ws, float T)
{
  float slow_rise = 0;
  float ws;

  while (true)
  {
    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, slow_rise, T);
    motor->set_voltage(voltage_pid);

    printf("slowStart,%lld,%f,%f,%f\n", get_absolute_time(), ws, slow_rise, voltage_pid);

    sleep_ms(200);
    slow_rise += 0.1f;

    if (slow_rise >= wanted_ws)
    {
      wanted_ws = slow_rise;
      printf("done with slow start\n");
      return true;
    }
  }
}

int main()
{
  stdio_init_all();
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD);
  sleep_ms(5000);
  printf("begin");
  slowStart(encoder, motor, pid, wanted_ws, T);

  while (true)
  {

    printf("%lld,%f,%f, \n", get_absolute_time(), ws, voltage_pid);

    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
  };
}
