#include "encoder.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <cstdio>

void step_response(Encoder *enc)
{
  printf("time(us),Degree pr Sec\n");
  while (true)
  {

    printf("%i,%f\n", get_absolute_time(), enc->get_ws());
  }
}
