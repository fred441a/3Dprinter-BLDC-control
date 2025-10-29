#define __FREERTOS 1
#include <FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

#define PWM 16
#define encoder 17
#define current 28

//HAS TO BE POWER OF 2!!!! 2^4=16
#define resolution 16
#define pwr_res 4
#define pwm_period_ms 1.112  //(1/899)*1000
#define debug

QueueHandle_t qPWM_measure;
void Calculate_PWM(int sum) {
  int avrg = (sum * 1000) >> pwr_res;
#ifdef debug
  Serial.printf("avr: %i, sum: %i \n", avrg, sum);
#endif
  BaseType_t err;
  err = xQueueSend(qPWM_measure, &avrg, 10);
#ifdef debug
  if (err == pdPASS) {
    switch (err) {
      case errQUEUE_BLOCKED:
        Serial.println("blocked");
        break;
      case errQUEUE_FULL:
        Serial.println("FULL");
        break;
      case errQUEUE_YIELD:
        Serial.println("yield");
        break;
    }
  }
#endif
}

void measure_PWM(void *pvParam) {
  for (;;) {
    static int sum = 0;
    static int i = 0;
    const static float delay_time = (pwm_period_ms / resolution) / portTICK_RATE_MS;

    sum += digitalRead(encoder);
    i++;

    if (i > resolution) {
      Calculate_PWM(sum);
      sum = 0;
      i = 0;
    }

    vTaskDelay(delay_time);
  }
}

void PID() {
  for (;;) {
    int avrg;
    xQueueReceive(qPWM_measure, &avrg, 10);
    vTaskDelay(pwm_period_ms / portTICK_RATE_MS);
  }
};

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
  Serial.printf("SerialDebuggin beginning \n");
#endif

  TaskHandle_t hmeasure_PWM;

  xTaskCreate(measure_PWM, "measPWM", 500, (void *)0, 0, &hmeasure_PWM);

#ifdef debug
  task_status();
#endif
}

void loop() {
  vTaskDelay(99999999);
}
