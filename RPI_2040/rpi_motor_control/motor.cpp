#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include <cstdio>
#include <sys/types.h>
#include <math.h>

class Motor {
private:
  uint gpio;
  uint dirPin;
  uint modePin;
  bool phaseMode;
  const uint count_top = 10000;
  const uint pwm_mult = count_top + 1;
  const uint max_voltage = 6;
  const float limit = 3;

public:
  Motor() { Motor(16, 17, 15, true); };
  Motor(uint pin, uint dirPin, uint modePin, bool phaseMode) : gpio(pin), dirPin(dirPin), modePin(modePin), phaseMode(phaseMode) {
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_A);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, count_top);
    pwm_config_set_clkdiv(&cfg, 200);
    pwm_init(pwm_gpio_to_slice_num(gpio), &cfg, true);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    gpio_init(dirPin);
    gpio_set_dir(dirPin, GPIO_OUT);
    gpio_put(dirPin, false);
    gpio_init(modePin);
    gpio_set_dir(modePin, GPIO_OUT);
    gpio_put(modePin, phaseMode);
  };
  void set_voltage(float v) {
    /*
    if (fabsf(v) > max_voltage ) {
      pwm_set_gpio_level(gpio, (uint16_t)(pwm_mult));
      if (v > 0){
        gpio_put(dirPin, false);
      }

      return;
    };
    */
      
    //limit for no crazy side extrusion
    if (fabsf(v) > limit){
      if (v < 0){
        v = -limit;
      }
      else{
      v = limit;
      }
    }

    float pwm_perc = (v / max_voltage);
    pwm_set_gpio_level(gpio, (uint16_t)(pwm_perc * pwm_mult));
    if (v < 0 ){
      gpio_put(dirPin, false);
    }
    else{
    gpio_put(dirPin, true);  
    }
  };

};