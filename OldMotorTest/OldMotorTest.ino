const int INA = 9;
const int INB = 10;
const int PWM = 11;
int speed = 0;

void setup(){

  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(PWM, OUTPUT);

  Serial.begin(9600);
 }

void loop(){

  digitalWrite(INA,HIGH); // Set INA pin output; Clockwise
  digitalWrite(INB,LOW); // Set INB pin output; Counterclockwise

  for (speed = 0; speed < 150; speed++) {
     analogWrite(PWM,speed);
     delay(20);
  }

  for (speed = 150; speed > 0; speed--) {
     analogWrite(PWM,speed);
     delay(20);
  }

  digitalWrite(INA,LOW); // Set INA pin output; Clockwise
  digitalWrite(INB,HIGH); // Set INB pin output; Counterclockwise

  for (speed = 0; speed < 150; speed++) {
     analogWrite(PWM,speed);
     delay(20);
  }

  for (speed = 150; speed > 0; speed--) {
     analogWrite(PWM,speed);
     delay(20);
  }

 }

