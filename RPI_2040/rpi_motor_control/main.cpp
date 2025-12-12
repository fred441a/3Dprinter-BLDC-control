#include "crc.cpp"
#include "encoder.cpp"
#include "motor.cpp"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"
#include <mutex>

// const uint gpio = 17;
std::mutex *ws_lock = new std::mutex;
float wanted_ws = 12;
const float T = 0.0105;
int c = 1;

float KP = 0.11;
float KI = 1.42;
float KD = 0;

#pragma pack(0)
struct Rx_Packet {
  int16_t ws;
  uint8_t crc;
};

void core1_main() {
  cyw43_arch_init();
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
  printf("starting core2");

  uart_init(uart1, 115200);
  gpio_set_function(8, UART_FUNCSEL_NUM(uart1, 0));
  gpio_set_function(9, UART_FUNCSEL_NUM(uart1, 1));

  // struct Rx_Packet send;
  //  send.ws = 115;
  //  send.crc = CRC8((const char *)&send, 2);
  char recieve[3];

  while (true) {
    if (uart_is_readable(uart1)) {
      uart_read_blocking(uart1, (uint8_t *)&recieve, 3);
      printf("%i\n", (int)((recieve[2] << 8) | recieve[1]));
    }
  }
}

int main() {
  stdio_init_all();
  multicore_launch_core1(core1_main);
  Encoder *encoder = new Encoder(5, 4);
  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD);
  sleep_ms(5000);
  while (true) {
    float ws;
    static float voltage_pid = 0;
    // printf("%lld,%f,%f \n", get_absolute_time(), ws, voltage_pid);

    ws = encoder->get_ws();
    voltage_pid = pid->voltageDis(ws, wanted_ws, T);
    motor->set_voltage(voltage_pid);
  }
}
