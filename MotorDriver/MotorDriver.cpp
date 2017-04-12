#include "MotorDriver.h"

const int INA = 8;
const int INB = 10;
const int PWM_PIN = 9;
int speed = 0;

void setup(){

  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  Serial.begin(9600);
 }

void loop(){

  digitalWrite(8,HIGH); // Set INA pin output; Clockwise
  digitalWrite(10,LOW); // Set INB pin output; Counterclockwise

  for (speed = 0; speed < 150; speed++) {
     analogWrite(9,speed);
     delay(20);
  }

  for (speed = 150; speed > 0; speed--) {
     analogWrite(9,speed);
     delay(20);
  }

  digitalWrite(8,LOW); // Set INA pin output; Clockwise
  digitalWrite(10,HIGH); // Set INB pin output; Counterclockwise

  for (speed = 0; speed < 150; speed++) {
     analogWrite(9,speed);
     delay(20);
  }

  for (speed = 150; speed > 0; speed--) {
     analogWrite(9,speed);
     delay(20);
  }

 }


