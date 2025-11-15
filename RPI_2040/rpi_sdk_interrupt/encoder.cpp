/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include <stdio.h>
#include <sys/types.h>

#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

class Encoder {
private:
  const float sample_time = 0.0011;
  uint gpio;
  float map(float input) { return (input * 379.346) - 7.966; };
  float measure_duty_cycle() {
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

public:
  Encoder() : gpio(17) {}
  Encoder(uint pin) : gpio(pin) {}

  float get_raw() { return measure_duty_cycle(); };
  float get_degree() { return map(measure_duty_cycle()); };
  float get_radian() { return map(measure_duty_cycle()) * 0.01745329; };
  float get_degPs() {
    float meas = get_degrees();
    sleep_us(sample_time * pow(10, 6));
    float meas2 = get_degrees();
    if (meas > meas2 && abs(meas - meas2) > 30) {
      return ((1 - abs(meas - meas2)) * (1 / sample_time));
    } else {
      return (meas2 - meas) * (1 / sample_time);
    }
  };

  float get_ws() {
    float meas = get_radians();
    sleep_us(sample_time * pow(10, 6));
    float meas2 = get_radians();
    if (meas > meas2 && abs(meas - meas2) > 0.5235987) {
      return (1 - abs(meas - meas2)) * (1 / sample_time);
    } else {
      return (meas2 - meas) * (1 / sample_time);
    }
  };
};
