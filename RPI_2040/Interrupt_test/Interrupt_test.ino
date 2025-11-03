unsigned long start = 0;
unsigned long pwm_time = 0;
const byte PWM = 17;

void up() {
  start = micros();
}

void down() {
  pwm_time = micros() - start;
}

void setup() {
  pinMode(PWM, INPUT_PULLDOWN);
  Serial.begin(115200);
  delay(3000);
  Serial.println("Hello-world");
  attachInterrupt(digitalPinToInterrupt(PWM), down, FALLING);
  attachInterrupt(digitalPinToInterrupt(PWM), up, RISING);
}
void loop() {
  Serial.printf("%li \n", pwm_time);
  delay(500);
}
