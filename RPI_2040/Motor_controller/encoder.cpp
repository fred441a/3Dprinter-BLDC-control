#include <Arduino.h>
#include <FreeRTOS.h>
#include <cstdlib>
#include <math.h>
#define pi 3.14159

class Encoder {
private:
  int pin;
  int sample_period_us;
  int resolution;
  // the power of 2 used to calculate the resolution
  int pwr_res;
  int sum = 0;
  // int *samples;

  /**
   * private function that pauses scheduling and samples pwm
   */
  void sample() {
    vTaskSuspendAll();
    sum = 0;
    for (int i = 0; i < resolution; i++) {
      sum += digitalRead(this->pin);
      // this->samples[i] = digitalRead(this->pin);
      delayMicroseconds(this->sample_period_us);
    }
    xTaskResumeAll();
  };

public:
  /**
   * encoder Constructor
   * @param pin the input pin of the encoder pwm
   * @param pwm_us the PWM frequecy in Hz
   * @param pwr_res the power of 2 specifying the resolution fx '2⁴ = 16'
   * @param max_speed the max angular momentum in RPM
   * pwr_res=4 will measure the signal 16 times in 1 period of the pwm
   */
  Encoder(int pin, int pwm_freq, int pwr_res)
      : pin(pin), pwr_res(pwr_res), resolution(pow(2, pwr_res)),
        sample_period_us(pow(10, 6) / (pwm_freq * pow(2, pwr_res))) {
    // this->samples = (int *)malloc(resolution * sizeof(int));
    pinMode(this->pin, INPUT_PULLDOWN);
  };
  /**
   * encoder class deconstructer (probably not really needed)
   */
  //~encoder() { free(this->samples); };
  /**
   * returns the current angle in degrees
   */
  float getDegree() {
    this->sample();
    return ((sum * 1000) >> pwr_res) * 36.0;
  };
  /**
   *returns the current angle in radians
   */
  float getRadians() {
    const static int rot = (2 * pi) / 1000.0;
    this->sample();
    return ((sum * 1000) >> pwr_res) * rot;
  };
  /**
   * returns the angular momentum (in rad/s)
   */
  float getRadpS() {
    const int wait_for = 1 / portTICK_PERIOD_MS;
    int rad1 = getRadians();
    vTaskDelay(wait_for);
    int rad2 = getRadians();

    return abs(rad2 - rad1) * 1000;
  };
  /**
   * returns the angular momentum (in deg/s)
   */
  // float getDegPs();
  /**
   * retuns the angular momentum (in RPM)
   */
  // float getRPM();
};
