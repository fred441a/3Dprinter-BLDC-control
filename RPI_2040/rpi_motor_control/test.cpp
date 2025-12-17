#include "motor.cpp"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pid.cpp"
#include "step_response.cpp"
#include "hardware/i2c.h"

// const uint gpio = 17;
float wanted_ws = 9;
const float T = 0.01025;
int c = 1;

float KP = 0.11;
float KI = 0.5;
float KD = 0;

float measure_w() {
  uint8_t buff[2];
  uint8_t data = 0x0E;
  i2c_write_blocking(i2c_default, 0x36, &data, 1, true);
  i2c_read_burst_blocking(i2c_default, 0x36, buff, 2);

  int raw = ((buff[0] << 8) | buff[1]);
  return 0.00153398078789 * raw;
}

float measure_ws() {
  float w1 = measure_w();
  sleep_ms(10);
  float w2 = measure_w();
  return (w2 - w1) / 0.01;
}

void setup_i2c() {
  i2c_init(i2c_default, 100 * 1000);
  i2c_set_baudrate(i2c_default, 100 * 1000);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

int main() {
  stdio_init_all();
  setup_i2c();
  Encoder *encoder = new Encoder(19);

  Motor *motor = new Motor(16, 0.3599);
  PID *pid = new PID(KP, KI, KD);
  for (float i = 0; i < 6; i += 0.5) {
    motor->set_voltage(i);
    sleep_ms(1000);
    float meas = measure_ws();
    motor->set_voltage(i + 1);
    sleep_ms(1000);
    float meas2 = measure_ws();
    printf("Coef %f %f %f\n", meas2 / meas, meas, meas2);
  }

  /*
    printf("absolute time [uS], PID voltage, measured ws, wantede WS \n");
    while (true) {

      float ws;
      if (c > 100) {
        wanted_ws = 9;
      } else {
        wanted_ws = 0;
        c += c++;
      }
      ws = encoder->get_ws();
      motor->set_voltage(wanted_ws);
      float voltage = pid->voltageDis(ws, wanted_ws, T);
      printf("%lld,%f,%f, %f\n", get_absolute_time(), voltage, ws, wanted_ws);
      motor->set_voltage(voltage);
    }
    */
}
