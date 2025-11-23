#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
float wanted_ws = 3;
int c = 0;
const float T = 0.01;

int main()
{
  stdio_init_all();
  sleep_ms(10000);
  Encoder *encoder = new Encoder(19);

  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(0.0511, 1.768, 0);
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
    printf("abstime(us), VoltagePID, Degree pr Sec, Wanted WS:%f,,\n", wanted_ws);
    for (int c = 0; c <= 50; c++)
    {
      float ws;
      ws = encoder->get_ws();
      // float voltage = pid->voltage(ws, wanted_ws);
      float voltage2 = pid->voltageDis(ws, wanted_ws, T);
      printf("%lld,%f,%f,,,\n", get_absolute_time(), voltage2, ws);
      motor->set_voltage(voltage2);
    }
    wanted_ws = wanted_ws + 1;
    sleep_ms(3000);
    if (wanted_ws >= 14){
      wanted_ws = 3;
    }
  }

  return 0;
}
