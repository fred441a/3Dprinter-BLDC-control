const int resolution = 16;
const int encoder = 17;

void setup() {
  pinMode(encoder, INPUT_PULLDOWN);
  Serial.begin(115200);
}

void loop() {
  int sum = 0;
  int i = 0;
  while (i < resolution) {
    sum += digitalRead(encoder);
    i++;
    delayMicroseconds(69);
  }
  Serial.println(sum*1000 / 16);
}
