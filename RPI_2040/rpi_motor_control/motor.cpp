#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include <cstdio>
#include <sys/types.h>

class Motor {
private:
  uint gpio;
  uint direction;
  uint modePin;
  bool phaseMode;
  float Ke;
  const uint count_top = 10000;
  const uint pwm_mult = count_top + 1;
  const uint max_voltage = 6;
  const float limit = 3;

public:
  Motor() { Motor(17, 16, 15, true, 0.07); };
  Motor(uint pin, uint direction, uint modePin, bool phaseMode,float Ke) : gpio(pin), direction(direction), modePin(modePin), phaseMode(phaseMode), Ke(Ke) {
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_A);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, count_top);
    pwm_config_set_clkdiv(&cfg, 200);
    pwm_init(pwm_gpio_to_slice_num(gpio), &cfg, true);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    gpio_init(direction);
    gpio_set_dir(direction, GPIO_OUT);
    gpio_put(direction, false);
    gpio_init(modePin);
    gpio_set_dir(modePin, GPIO_OUT);
    gpio_put(modePin, phaseMode);
    
  };
  void set_speed(float ws) {
    float pwm_perc = Ke / (max_voltage * ws);
    pwm_set_gpio_level(gpio, (uint16_t)(pwm_perc * pwm_mult));
    gpio_put(direction, false); //If reverse is wanted, it should be something else here!
  };
  void set_voltage(float v) {
    if (v > max_voltage) {
      pwm_set_gpio_level(gpio, (uint16_t)(pwm_mult));
      return;
    };
    if (v < 0) {
      pwm_set_gpio_level(gpio, (uint16_t)0);
      return;
    }
    //limit for no crazy side extrusion
    if (v > limit){
      v = limit;
    }
    float pwm_perc = (v / max_voltage);
    pwm_set_gpio_level(gpio, (uint16_t)(pwm_perc * pwm_mult));
  };

};
