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
  const uint SDA, SCL;
  const float fsSample_i2c_ms = 0.571;
  const float delay_us = 9430;
  float map_pos2rad(uint16_t input) { return 0.0015 * input; };
  const int baudRate = 100000;

public:
  Encoder(uint SCL, uint SDA) : SCL(SCL), SDA(SDA)
  {
    i2c_init(i2c_default, baudRate);
    i2c_set_baudrate(i2c_default, baudRate);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(SDA, SCL, GPIO_FUNC_I2C));
  };

  float get_rad() { return map_pos2rad(get_ang_raw()); };

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

  float get_ws()
  {
    static float ws = 0;
    static float last_ws;
    uint16_t posDiff = 0;
    uint16_t meas1 = get_ang_raw();
    printf("%lld,", get_absolute_time());
    sleep_us(delay_us);
    uint16_t meas2 = get_ang_raw();
    printf("%lld,", get_absolute_time());

    if (meas1 > meas2)
    {
      ws = last_ws;
    }
    else
    {
      posDiff = meas2 - meas1;
      // printf("%u, ", posDiff);
      float possDiff_rad = (map_pos2rad(posDiff));
      ws = possDiff_rad * 1000 / (10);
    }
    last_ws = ws;
    return ws;
  };
  /*
  float get_ws_rad()
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
*/
  
};