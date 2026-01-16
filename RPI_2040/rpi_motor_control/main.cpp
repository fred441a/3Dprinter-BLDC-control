#include "encoder.cpp"
#include "motor.cpp"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "protokol.cpp"
#include "step.cpp"
#include "step_response.cpp"

// const uint gpio = 17;

std::queue<float> ws_recieved;
std::queue<bool> alarm_queue;
float wanted_ws = 7.2;
const float T = 0.0105;
int c = 1;

static float slow_rise = 0;
static float ws = 0;
static float voltage_pid;

float KP = 0.13;
float KI = 1.428;
float KD = 0;

void core1_main() {
  Step *measure_step = new Step(&ws_recieved, &alarm_queue);
  measure_step->loop();
}

// slow start function:
bool slowStart(Encoder *encoder, Motor *motor, PID *pid, float T,
               float wanted_ws, float ws, float slow_rise) {

  while (true) {
    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, slow_rise, T);
    motor->set_voltage(voltage_pid);

    printf("slowStart,%lld,%f,%f,%f\n", get_absolute_time(), ws, slow_rise,
           voltage_pid);

    slow_rise += 0.00001f;

    if (slow_rise >= wanted_ws) {
      wanted_ws = slow_rise;
      printf("done with slow start\n");
      return true;
    }
  }
}

int main() {
  stdio_init_all();
  multicore_launch_core1(core1_main);
  sleep_us(1000);
  printf("godmorgen");
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(17, 16, 15, true, 0.3599);
  PID *pid = new PID(KP, KI, KD, &alarm_queue);
  while (true) {
    float ws;
    static float voltage_pid = 0;

    ws = encoder->get_ws();
    if (!ws_recieved.empty()) {
      wanted_ws = ws_recieved.front();
      ws_recieved.pop();
    }
    voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
  };
}
