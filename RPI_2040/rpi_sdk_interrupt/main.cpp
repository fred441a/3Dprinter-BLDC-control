/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include <stdio.h>

#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define NUM_SECTIONS 2


const uint MEASURE_PIN = 17;

// maps the input percent to an actual degree
float map(float input) { return input * 382.165; };

float measure_duty_cycle(uint gpio) {
  // Only the PWM B pins can be used as inputs.
  assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
  uint slice_num = pwm_gpio_to_slice_num(gpio);

  // Count once for every 100 cycles the PWM B input is high
  pwm_config cfg = pwm_get_default_config();
  pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
  pwm_config_set_clkdiv(&cfg, 100);
  pwm_init(slice_num, &cfg, false);
  gpio_set_function(gpio, GPIO_FUNC_PWM);

  pwm_set_enabled(slice_num, true);
  sleep_ms(10);
  pwm_set_enabled(slice_num, false);
  float counting_rate = clock_get_hz(clk_sys) / 100;
  float max_possible_count = counting_rate * 0.01;
  return pwm_get_counter(slice_num) / max_possible_count;
}

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
  printf("RAW,DEGREE,ANGULAR_MOMENTUM\n");

  for (;;) {
    static float old_meas = 0;
    float raw = measure_duty_cycle(MEASURE_PIN);
    float degree = map(raw);
    float angular_momentum;

    if (old_meas > raw && abs(old_meas - raw) > 0.1) {
      angular_momentum = 360 - (map(old_meas) - degree);
    } else {
      angular_momentum = degree - map(old_meas)/909.1;
    }
	old_meas = raw;
    sleep_us(1100);

    float y = iir_filter(angular_momentum);
    printf("in=%f out=%f\n", angular_momentum, y);

    printf("%f,%f,%f\n", raw, degree, angular_momentum);
  }
}
