#include "pico/util/queue.h"
#include <mutex>
#include <queue>

class steps_to_radians {
private:
  queue_t *recieved_ws;
  queue_t *alarm_queue;

  const int step_pin = 22;
  static float rads;
  static constexpr float RadK = 0.002895809913542538;

public:
  steps_to_radians(queue_t *recieved_ws, queue_t *alarm_queue)
      : recieved_ws(recieved_ws), alarm_queue(alarm_queue) {
    gpio_init(step_pin);

    irq_set_exclusive_handler(IO_IRQ_BANK0, &this->callback);

    gpio_set_irq_enabled_with_callback(step_pin, GPIO_IRQ_EDGE_RISE, true,
                                       &this->callback);
  }
  static void callback(unsigned int gpio, long unsigned int events) {
    if (gpio == step_pin && events == 0x8) {
      static long int last_time = 0;
      long int step_time = (get_absolute_time() - last_time);
      rads = RadK / step_time;
    }
  }

  void loop() {
    for (;;) {
      queue_add_blocking(recieved_ws, &rads);
      sleep_us(1000);
    }
  }
};
