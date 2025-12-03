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
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#pragma once
class Encoder
{
private:
  const float sample_time = 0.01025;
  const float fsSample_i2c_ms = 0.24;
  const float delay_ms = 2;
  const float sample_time_ms = sample_time * pow(10, 3);
  uint gpio, slice_num;
  float map_degree(float input) { return 379.33 * input - 11.056; };
  float map_radians(float input) { return 6.6712 * input - 0.1929; };
  float map_pos2rad(uint16_t input) { return 0.0015 * input; };

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

  float get_rad() { return map_pos2rad(get_ang_raw()); };

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
    printf("%f\n", meas2);
    float out;

    if (meas > meas2 && abs(meas - meas2) > 0.5235987)
    {
      out = out_last; // = (6.2 - abs(meas - meas2)) / sample_time;
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

  void init_i2c()
  {
    i2c_init(i2c_default, 100 * 1000);
    i2c_set_baudrate(i2c_default, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(4, 5, GPIO_FUNC_I2C));
  };

  uint16_t get_ang_raw()
  {
    uint8_t buff[2];
    uint8_t data = 0x0E;
    int ret = 0;
    ret = i2c_write_blocking(i2c_default, 0x36, &data, 1, true);
    ret = i2c_read_burst_blocking(i2c_default, 0x36, buff, 2);
    uint16_t raw_angel = ((buff[0] << 8) | buff[1]);
    return raw_angel;
  };

  float get_wsGood()
  {
    static float ws = 0;
    static float last_ws;
    uint16_t posDiff = 0;
    uint16_t meas1 = get_ang_raw();
    sleep_ms(delay_ms);
    uint16_t meas2 = get_ang_raw();

    if (meas1 > meas2)
    {
      ws = last_ws;
    }
    else
    {
      posDiff = meas2 - meas1;
      float possDiff_rad = (map_pos2rad(posDiff));
      printf("possDiff:%u \n", posDiff);
      ws = possDiff_rad * 1000 / (delay_ms + fsSample_i2c_ms);
    }
    last_ws = ws;
    return ws;
  };

  float get_wsGood_radBased()
  {
    static float ws = 0;
    static float last_ws;
    float posDiff = 0;
    float meas1 = get_rad();
    sleep_ms(delay_ms);
    float meas2 = get_rad();

    if (meas1 > meas2)
    {
      ws = last_ws;
    }
    else
    {
      posDiff = meas2 - meas1;
      ws = posDiff * 1000 / (delay_ms + fsSample_i2c_ms);
    }
    last_ws = ws;
    return ws;
  };
};
