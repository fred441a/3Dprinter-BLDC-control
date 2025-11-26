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

int main()
{
  stdio_init_all();
  sleep_ms(10000);
  Encoder *encoder = new Encoder(19);

  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(0.085, 1.4286, 0);
  /*
  motor->set_voltage(2);
  step_response(encoder);
  */

  /*
  for (float i = 0; i <= 5; i = i + 0.25)
  {
    float ws;
      motor->set_voltage(i);

      // wait for some time for motor to reach steady speed
      for (int j = 0; j < 500; j++)
      {
      ws = encoder->get_ws(); // read speed after settling
      }

      printf("i:%f, ws:%f \n", i, ws);
  }
  */

  while (true)
  {
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
    // float voltage = pid->voltage(ws, wanted_ws);
    float voltage2 = pid->voltageDis(ws, wanted_ws, T);
    printf("%lld,%f,%f, %f\n", get_absolute_time(), voltage2, ws, wanted_ws);
    motor->set_voltage(voltage2);
  }
}