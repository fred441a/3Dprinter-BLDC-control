#define __FREERTOS 1
#include <FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

#define PWM 16
#define encoder 17
#define current 28

//HAS TO BE POWER OF 2!!!! 2^4=16
#define resolution 16
#define pwm_period_ms 1112  //(1/899)*1000



QueueHandle_t qPWM_measure;
SemaphoreHandle_t lPWM_measure_data;
int PWM_measure_data[resolution];


void measure_PWM(void *pvParam) {
  static int i = 0;
  const static int delay_time = (pwm_period_ms / resolution) / portTICK_RATE_MS;

  if (i > resolution) {
    i = 0;
  }
  PWM_measure_data[i] = digitalRead(encoder);
  i++;

  vTaskDelay(delay_time);
}

void Calculate_PWM() {

  vTaskDelay(pwm_period_ms / portTICK_RATE_MS);
}

void PID() {};


void setup() {
  //init
  qPWM_measure = xQueueCreate(1, sizeof(int));
  lPWM_measure_data = xSemaphoreCreateBinary();
  for (int i = 0; i > resolution; i++) {
    PWM_measure_data[i] = 0;
  };

  TaskHandle_t hmeasure_PWM;

  xTaskCreate(measure_PWM, "measure_PWM", 200, (void *)0, 0, &hmeasure_PWM);
}

void loop() {
}