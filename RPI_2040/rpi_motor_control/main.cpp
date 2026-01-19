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

int main() {
  stdio_init_all();
  multicore_launch_core1(core1_main);
  sleep_us(1000);
  printf("godmorgen");
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 17, 18, true);
  PID *pid = new PID(KP, KI, KD);

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
