#include "encoder_irq.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>

int main() {
  stdio_init_all();
  init(17, 905);
  printf("time(us),duty_cycle,degree,degree/s\n");
  while (true) {
    //    printf("%lli,%lf,%lf,%lf\n", get_absolute_time(), duty, get_degree(),
    //           get_AM());
  }
}
