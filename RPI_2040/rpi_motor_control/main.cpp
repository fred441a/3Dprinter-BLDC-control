#include "encoder.cpp"
#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"

// const uint gpio = 17;
const float wanted_ws = 15;
int c = 0;
const float T = 0.02;

int main()
{
  stdio_init_all();
  sleep_ms(10000);
  Encoder *encoder = new Encoder(19);

  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(0.0511, 1.767, 0);
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
    ws = encoder->get_ws();
    // float voltage = pid->voltage(ws, wanted_ws);
    float voltage2 = pid->voltageDis(ws, wanted_ws, T);
    printf("%f,:%f,%i\n", voltage2, ws, get_absolute_time());
    motor->set_voltage(voltage2);
    // sleep_ms(1000);

    c = c +1;
    if (c > 100){
      return false;

    }
    
  }

  return 0;
}
