#include "test_class.h"

#define PWM_pin 2
#define Feedback_pin 16

test *testClass;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  testClass = new test();

  //set PWM freq
  analogWriteFreq(200);

  //Turn on LED when booted
  digitalWrite(LED_BUILTIN,HIGH);

  testClass->print();
}

void loop() {
  analogWrite(50,PWM_pin);
  testClass->print();
  delay(50);
}
