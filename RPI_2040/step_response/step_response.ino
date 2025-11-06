const int resolution = 64;
const int encoder = 17;

void setup() {
  pinMode(encoder, INPUT_PULLDOWN);
  Serial.begin(115200);
}

int get_angle() {

  int sum = 0;
  int i = 0;
  while (i < resolution*4) {
    sum += digitalRead(encoder);
    i++;
    delayMicroseconds(17);
  }

  return (sum * 1000 / (resolution*4));
}

void loop() {
  static int angle, angle2 = 0;
  angle = get_angle();
  delay(100);
  angle2 = get_angle();
  //Serial.printf("angle: %i,angle2: %i,speed: %i \n",angle,angle2,abs(angle2-angle));
  int speed = abs(angle2-angle);
  if(angle > angle2){
    speed = (1000-angle)+angle2;
  };
  Serial.println(speed);
}

void loop() {
  static int angle, angle2 = 0;
  angle = get_angle();
  delay(100);
  angle2 = get_angle();
  //Serial.printf("angle: %i,angle2: %i,speed: %i \n",angle,angle2,abs(angle2-angle));
  int speed = abs(angle2-angle);
  if(angle > angle2){
    speed = (1000-angle)+angle2;
  };
  Serial.println(speed);
}