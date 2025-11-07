const int resolution = 128;
const int encoder = 17;
const int flipPin = 16;
const int samSize = 8;
bool stillStand = 0;
float speedRpm = 0;

const int Tlow = 25;
const int Thigh = 40;
const int range = 5;
int baseShift = 32;
int baseDelay = 50;
int DC = 0;

int delayArray[5] = { 70, 50, 30, 20, 1 };

const int samTime_mS = 65;

int posArray1[samSize];
int posArray2[samSize];
int diffPossA[samSize];

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

  int d = 0;


  for (d = 0; d < 5; d++) {
    int t = 0;
    speedRpm = 0;

    for (t = 0; t < 100; t++) {
      static int angle, angle2 = 0;
      int i = 0;
      int j = 0;



      int sumDiffPoss = 0;
      float avrDiffPoss;
      int eCounter = 0;

      for (i = 0; i < samSize; i++) {
        posArray1[i] = get_angle();
        delay(1);
      }
      //Serial.printf("posArray1_7:%i \n", posArray1[7]);

      i = 0;
      delay((delayArray[d]));

      //delay(30);
      for (i = 0; i < samSize; i++) {
        posArray2[i] = get_angle();
        delay(1);
      }
      //Serial.printf("posArray2_7:%i \n", posArray2[7]);

      for (j = 0; j < samSize; j++) {
        diffPossA[j] = abs(posArray2[j] - posArray1[j]);

        if (posArray1[j] > posArray2[j]) {

          if (diffPossA[j] < 15) {  //Need to make a new error detection.
            diffPossA[j] = 0;
            //Serial.println("error");
            eCounter += 1;

          } else if (diffPossA[j] > 200) {
            diffPossA[j] = (1000 - posArray1[j]) + posArray2[j] - 58;
            //Serial.println("loop");
          }
        }
        sumDiffPoss += diffPossA[j];
      }
      avrDiffPoss = (sumDiffPoss / (samSize - eCounter));

      speedRpm += (1/ ((((avrDiffPoss)) * 60) / (delayArray[d] + baseShift)) );
    }

    //Serial.printf("place1:%i \n ", posArray1[1]);
    //Serial.printf("avrDiffPoss:%f \n", avrDiffPoss);
    Serial.printf("delayArray:%i \n", delayArray[d]);
    Serial.printf("SpeedRpm:%f \n", (100 / speedRpm));
  }
  /*
  if (avrDiffPoss > Thigh) {
    if (DC < 60) {
      DC += 1;
    }
  }

  else if (avrDiffPoss < Tlow) {
    if (DC > 1)
      DC -= 1;
  }
  */
}