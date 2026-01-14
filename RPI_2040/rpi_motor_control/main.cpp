#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
const float ws_locked = 4.2;
float wanted_ws = 0.5;

const float T = 0.0107;

static float slow_rise = 0;
static float ws = 0;
static float voltage_pid;

float KP = 0.13;
float KI = 1.428;
float KD = 0;

int main()
{
  stdio_init_all();
  sleep_ms(5000);
  printf("1 \n");
  Encoder *encoder = new Encoder(5, 4);
  printf("2 \n");
  Motor *motor = new Motor(16, 17, 15, true, 0.3599);
  printf("3 \n");
  PID *pid = new PID(KP, KI, KD);
  printf("4 \n");
  printf("TimeStamp [us], angular velocity[rad/s] \n");
  //slowStart(encoder, motor, pid, T, wanted_ws, ws, slow_rise);

  while (true)
  {
    printf("%lld, %f \n", get_absolute_time(), encoder->get_ws());
    motor->set_voltage(1);
  };
}