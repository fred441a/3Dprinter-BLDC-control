#define __FREERTOS 1
#include <FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
#include "encoder.cpp"

#define PWM 16
#define current 28

//HAS TO BE POWER OF 2!!!! 2^4=16
#define resolution 16
#define pwr_res 4
#define pwm_period_ms 1.112  //(1/899)*1000
#define debug

QueueHandle_t qPWM_measure;
Encoder encoder(17, 899, 4);

void measure_PWM(void* params) {
  for (;;) {
    Serial.printf("deg: %f, ws: %f\n", encoder.getDegree(), encoder.getRadpS());
    vTaskDelay(500 / portTICK_RATE_MS);
  }
}


void task_status() {
  vTaskDelay(2000 / portTICK_RATE_MS);

  TaskStatus_t TaskDetails[10];

  uxTaskGetSystemState(TaskDetails, 10, NULL);
  //(hmeasure_PWM, &TaskDetails, pdTRUE, eInvalid);

  for (int i = 0; i < 10; i++) {
    Serial.printf("name: %s \nState: %i\nRemaining stack: %i\nRuntime: %i\n", TaskDetails[i].pcTaskName, TaskDetails[i].eCurrentState, TaskDetails[i].usStackHighWaterMark, TaskDetails[i].ulRunTimeCounter);
  }
}



void setup() {
  //init
  qPWM_measure = xQueueCreate(1, sizeof(int));

#ifdef debug
  Serial.begin(115200);
#endif

  TaskHandle_t hmeasure_PWM;

  xTaskCreate(measure_PWM, "measPWM", 500, (void*)0, 0, &hmeasure_PWM);

#ifdef debug
  task_status();
#endif
}

void loop() {
}
