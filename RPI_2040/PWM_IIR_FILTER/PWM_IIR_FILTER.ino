#define __FREERTOS 1
#include <FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

//pins
#define PWM 16
#define encoder 17
#define current 28

#define resolution 16
#define pwm_period_ms 1.112  //(1/899)*1000
#define debug

int samples[resolution];

void Calculate_PWM() {
  const double b[resolution] = {
    (3.073 * pow(10, -6)),
    (2.151 * pow(10,-5)),
    (6.453 * pow(10,-5)),
    (0.0001076),
    (0.0001076),
    (6.453 * pow(10,-5)),
    (2.151 * pow(10,-5)),
    (3.073 * pow(10, -6)),
  } ;

  const int a[resolution]

  for (;;) {
    vTaskDelay(pwm_period_ms / portTICK_RATE_MS);
    static y[resolution];
    for (int i = 0; i < resolution; i++) {
      y[n] =
    }

#ifdef debug
    //Serial.printf("avr: %i, sum: %i \n", avrg, sum);
#endif
  }
}

void measure_PWM(void *pvParam) {
  static int i = 0;
  const static float delay_time = (pwm_period_ms / resolution) / portTICK_RATE_MS;

  for (;;) {

    samples[i] = digitalRead(encoder);
    i++;

    vTaskDelay(delay_time);
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
  pinMode(encoder, INPUT_PULLDOWN);
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
