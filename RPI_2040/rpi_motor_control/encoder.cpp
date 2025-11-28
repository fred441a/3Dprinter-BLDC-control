/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cstdio>
#include <math.h>
#include <sys/types.h>

#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#pragma once
class Encoder
{
private:
  const float sample_time = 0.01025;
  const float sample_time_ms = sample_time * pow(10, 3);
  uint gpio, slice_num;
  float map_degree(float input) { return 379.33 * input - 11.056; };
  float map_radians(float input) { return 6.6712 * input - 0.1929; };

  float measure_duty_cycle()
  {
    //    pwm_set_enabled(slice_num, true);

    pwm_set_counter(slice_num, 0);
    sleep_ms(sample_time_ms);
    return pwm_get_counter(slice_num) /
           (float)(clock_get_hz(clk_sys) / 100 * sample_time);
  }

public:
  Encoder() { Encoder(17); }
  Encoder(uint pin) : gpio(pin)
  {
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
    slice_num = pwm_gpio_to_slice_num(gpio);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
    pwm_config_set_clkdiv(&cfg, 100);
    pwm_init(slice_num, &cfg, true);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
  }

  float get_raw() { return measure_duty_cycle(); };
  float get_degree() { return map_degree(measure_duty_cycle()); };
  float get_radian() { return map_radians(measure_duty_cycle()); };
  float get_degPs()
  {
    float meas = get_degree();
    float meas2 = get_degree();
    if (meas > meas2 && abs(meas - meas2) > 30)
    {
      return ((1 - abs(meas - meas2)) / sample_time);
    }
    else
    {
      return (meas2 - meas) / sample_time;
    }
  };

  float get_ws()
  {
    static float out_last = 0;
    static float meas = 0;
    float meas2 = get_radian();
	printf("%f\n",meas2);
    float out;
    
    if (meas > meas2 && abs(meas - meas2) > 0.5235987)
    {
      out = out_last;// = (6.2 - abs(meas - meas2)) / sample_time;
    }
    else
    {
      out = (meas2 - meas) / sample_time;
    }
    if (out < 0 || out > 19)
    {
      return out_last;
    }
    out_last = out;
	meas = meas2;
    return out;
  };
};
