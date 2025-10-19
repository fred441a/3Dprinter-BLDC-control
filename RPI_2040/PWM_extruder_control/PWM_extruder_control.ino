#include <FreeRTOS.h>
#define PWM_pin 16


void setup() {
  pinMode(PWM_pin,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  //set PWM freq
  analogWriteFreq(200);

  //Turn on LED when booted
  digitalWrite(LED_BUILTIN,HIGH);
}

void loop() {

//  digitalWrite(PWM_pin,LOW);
  delay(2000);
 // digitalWrite(PWM_pin,HIGH);
  delay(2000);

}
