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


// slow start function:
bool slowStart(Encoder *encoder, Motor *motor, PID *pid,  float T, float wanted_ws, float ws, float slow_rise)
{

  while (true)
  {
    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, slow_rise, T);
    motor->set_voltage(voltage_pid);

    printf("slowStart,%lld,%f,%f,%f\n", get_absolute_time(), ws, slow_rise, voltage_pid);

    slow_rise += 0.00001f;

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
  printf("TimeStamp, angular velocity[rad/s], voltage ['V'], wanted_ws \n");
  //slowStart(encoder, motor, pid, T, wanted_ws, ws, slow_rise);

  while (true)
  {
    
    if(wanted_ws < ws_locked){
      wanted_ws += 0.005f;
    }
    

    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
    printf("%lld,%f,%f,%f \n", get_absolute_time(), ws, voltage_pid, wanted_ws);

  };
}
