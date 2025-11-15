#include "encoder_irq.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>

#define NUM_SECTIONS 2

typedef struct {
    float b0, b1, b2;
    float a1, a2;
    float w1, w2;
} Biquad;

Biquad sections[NUM_SECTIONS] = {
    { 1.0000f,  1.2245f,  1.0000f,  -0.3490f,  0.3285f,  0, 0 },
    { 1.0000f,  0.2333f,  1.0000f,   0.0094f,  0.9023f,  0, 0 }
};

float overallGain = 1.0f;

static inline float biquad_process(Biquad *s, float x) {
    float y = s->b0 * x + s->w1;
    float new_w1 = s->b1 * x - s->a1 * y + s->w2;
    float new_w2 = s->b2 * x - s->a2 * y;
    s->w1 = new_w1;
    s->w2 = new_w2;
    return y;
}

float iir_filter(float x) {
    for (int i = 0; i < NUM_SECTIONS; ++i)
        x = biquad_process(&sections[i], x);
    return x * overallGain;
}


int main() {
  stdio_init_all();
  init(17, 905);
  printf("time(us),duty_cycle,degree,degree/s\n");
  while (true) {
    //    printf("%lli,%lf,%lf,%lf\n", get_absolute_time(), duty, get_degree(),
    //           get_AM());
  }
}
