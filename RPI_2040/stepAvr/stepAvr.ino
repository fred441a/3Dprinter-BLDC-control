const int resolution = 128;
const int encoder = 17;
const int flipPin = 16;
const int samSize = 32;

int posArray1[samSize];
int posArray2[samSize];
int speedArray[samSize];

void setup() {
  pinMode(encoder, INPUT_PULLDOWN);
  pinMode(flipPin, OUTPUT);
  Serial.begin(115200);
}

int get_pos(int encoder_pin) {
  int pos = digitalRead(encoder_pin);
  //float pos2 = analogRead(encoder_pin);
  //Serial.printf("analog:%i \n", pos2);
  //Serial.printf("position:%i \n", pos);
  return pos;
}

int get_angle() {

  int sum = 0;
  int i = 0;
  int sumScaled = 0;
  while (i < resolution) {
    delayMicroseconds(8);
    sum += get_pos(encoder);
    i++;
  }
  sumScaled = ((sum * 1000) / resolution);
  //Serial.printf("pos:%i \n", sumScaled);
  return (sumScaled);
}

void loop() {
  static int angle, angle2 = 0;
  int i = 0;
  int j = 0;
  int sumSpeed = 0;
  int avrSpeed;
  for (i = 0; i < samSize; i++) {
    posArray1[i] = get_angle();
    delay(1);
  }
  //Serial.printf("posArray1_7:%i \n", posArray1[7]);

  i = 0;
  delay(30);

  for (i = 0; i < samSize; i++) {
    posArray2[i] = get_angle();
    delay(1);
  }
  //Serial.printf("posArray2_7:%i \n", posArray2[7]);

  for (j = 0; j < samSize; j++) {
    speedArray[j] = abs(posArray2[j] - posArray1[j]);

    if (posArray1[j] > posArray2[j]) {

      if (speedArray[j] < 58 ) {
        speedArray[j] = avrSpeed;
        Serial.println("error");

      } else {
        speedArray[j] = (1000 - posArray1[j]) + posArray2[j] - 58;
      //Serial.print("loop");
      }
    }
    sumSpeed += speedArray[j];
  }
  avrSpeed = (sumSpeed / samSize);
  //Serial.printf("sumSpeed:%i \n", sumSpeed);
  Serial.printf("avrSpeed:%i \n", avrSpeed);
  Serial.printf("position:%i \n", posArray1[1]);

}