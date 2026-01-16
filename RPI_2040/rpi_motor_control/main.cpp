#include "encoder.cpp"
#include "motor.cpp"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"
#include "step_to_rad.cpp"

// const uint gpio = 17;

queue_t ws_recieved;
queue_t alarm_queue;
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
  queue_init(&ws_recieved, sizeof(float), 10);
  queue_init(&alarm_queue, sizeof(bool), 10);

  steps_to_radians *stepRadCalc =
      new steps_to_radians(&ws_recieved, &alarm_queue);
  stepRadCalc->loop();
}


int main() {
  sleep_ms(50000);
  printf("Are you ready? \n");
  stdio_init_all();
  multicore_launch_core1(core1_main);
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 17, 15, true);
  PID *pid = new PID(KP, KI, KD);
  printf("TimeStamp, angular velocity[rad/s], voltage ['V'] \n");
  // slowStart(encoder, motor, pid, T, wanted_ws, ws, slow_rise);

  while (true) {
    float ws;
    static float voltage_pid = 0;

    ws = encoder->get_ws();
    if (!queue_is_empty(&ws_recieved)) {
      queue_remove_blocking(&ws_recieved, &wanted_ws);
    }
    voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
    printf("time (us): %lld, wanted_ws :%f\n", get_absolute_time(), wanted_ws);
  };
}