/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include <sys/types.h>

#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

class Encoder {
private:
  const float sample_time_ms = 2.2;
  const float sample_time = 0.00022;
  uint gpio, slice_num;
  float map_degree(float input) { return 379.346 * input - 7.966; };
  float map_radians(float input) { return 66.2084 * input - 1.39033; };
  float measure_duty_cycle() {
    pwm_set_enabled(slice_num, true);
    sleep_ms(sample_time_ms);
    return pwm_get_counter(slice_num) / clock_get_hz(clk_sys) * sample_time;
  }

public:
  Encoder() { Encoder(17); }
  Encoder(uint pin) : gpio(pin) {
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
    slice_num = pwm_gpio_to_slice_num(gpio);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
    pwm_config_set_clkdiv(&cfg, 1);
    pwm_init(slice_num, &cfg, false);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
  }

  float get_raw() { return measure_duty_cycle(); };
  float get_degree() { return map_degree(measure_duty_cycle()); };
  float get_radian() { return map_radians(measure_duty_cycle()); };
  float get_degPs() {
    float meas = get_degree();
    float meas2 = get_degree();
    if (meas > meas2 && abs(meas - meas2) > 30) {
      return ((1 - abs(meas - meas2)) * (1 / sample_time));
    } else {
      return (meas2 - meas) * (1 / sample_time);
    }
  };

  float get_ws() {
    float meas = get_radian();
    float meas2 = get_radian();
    if (meas > meas2 && abs(meas - meas2) > 0.5235987) {
      return (1 - abs(meas - meas2)) * (1 / sample_time);
    } else {
      return (meas2 - meas) * (1 / sample_time);
    }
  };
};
