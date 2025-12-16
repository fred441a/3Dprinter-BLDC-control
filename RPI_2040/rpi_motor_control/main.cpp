#include "encoder.cpp"
#include "motor.cpp"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "protokol.cpp"
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

uint16_t value = 0x0000;
uint16_t second_value = 0xECC2;
char crc = CRC8((const char *)&value, 2);
char crc2 = CRC8((const char *)&second_value, 2);
printf("%04x%02x\n", value, crc);
printf("%04x%02x\n", second_value, crc2);


  Protokol *pr = new Protokol(&ws_recieved, &alarm_queue);
  pr->read_write_loop();
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

    sleep_ms(200);
    slow_rise += 0.1f;

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
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD, &alarm_queue);
  printf("TimeStamp, angular velocity[rad/s], voltage ['V'] \n");
  // slowStart(encoder, motor, pid, T, wanted_ws, ws, slow_rise);

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
    printf("time (us): %lld, wanted_ws :%f\n", get_absolute_time(), wanted_ws);
  };
}
