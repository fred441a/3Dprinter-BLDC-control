#include "crc.cpp"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <queue>

enum alarms { CRC_ERROR, NOZZLE_WORN, NOZZLE_VERY_WORN };

class Protokol {
private:
  std::queue<float> *recieved_ws;
  std::queue<bool> *alarm_queue;
  const size_t length = 3;

  uint16_t chartoint16(char *input) {
    return (uint16_t)((input[1] << 8) | input[0]);
  }

public:
  Protokol(std::queue<float> *recieved_ws, std::queue<bool> *alarm_queue)
      : recieved_ws(recieved_ws), alarm_queue(alarm_queue) {
    cyw43_arch_init();
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    uart_init(uart1, 115200);
    gpio_set_function(8, UART_FUNCSEL_NUM(uart1, 0));
    gpio_set_function(9, UART_FUNCSEL_NUM(uart1, 1));
  }

  void read_write_loop() {
    for (;;) {
      if (uart_is_readable(uart1)) {

        char recieved[length];
		//TODO det her er lidt fyfy, fordi vi sådan set kan låse hele den anden core
        uart_read_blocking(uart1, (uint8_t *)&recieved, length);
        uint16_t fixed_point_ws = chartoint16(&recieved[1]);
        if (check_crc(recieved)) {
          recieved_ws->push((float)fixed_point_ws / 1000);
        } else {
          Alarm(CRC_ERROR);
        }
      }
      if (!alarm_queue->empty()) {
        alarm_queue->pop();
        Alarm(NOZZLE_WORN);
      }
    }
  }

  bool check_crc(char *recieved) {
    uint16_t recieved_ptr = chartoint16(&recieved[1]);
    char check = CRC8((char *)&recieved_ptr, length - 1);
    if (check == recieved[0]) {
      return true;
    }
    return false;
  }

  void Alarm(alarms alarm) {
    char send_buf[3];
    send_buf[0] = alarm;
    send_buf[3] = CRC8(send_buf, 2);
    uart_write_blocking(uart1, (const uint8_t *)send_buf, 3);
  }
};
