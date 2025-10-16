#include <Arduino_FreeRTOS.h>
#define PWM_pin 2
#define Feedback_pin 16


void setup() {
  pinMode(2,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  //set PWM freq
  analogWriteFreq(200);

  //Turn on LED when booted
  digitalWrite(LED_BUILTIN,HIGH);

}

void loop() {}
