#include <TiltMotor.h>
#include <MotorDriver.h>

MotorDriver TiltMotor::T = MotorDriver();
int TiltMotor::S;
volatile bool TiltMotor::homed;
char* TiltMotor::message;


void TiltMotor::init() {
  T.init(A,B,P);
  pinMode(S, INPUT_PULLUP);
}

bool TiltMotor::home() {
  homed = false;
  Serial.println("homing");
  if (digitalRead(S)==LOW) {
    homed = true;
    pos = 0;
    return homed;
  }
  message = "finished home\n";
  attachInterrupt(digitalPinToInterrupt(S),finishHome, FALLING);
  T.forward(speed*3/4); //try tilting up first;
  delay(3000);
  if (homed) {
    pos = 0;
    return homed;
  }
  T.backward(speed*3/4); //if not homed yet, try tilting down.
  delay(3000);
  if (homed) pos = 0;
  return homed;
}

void TiltMotor::tiltUp(){
  if (pos>0) return;
  Serial.println("tilting up");
  message = "tilted up\n";
  attachInterrupt(digitalPinToInterrupt(S),finishTilt, RISING);
  T.forward(speed);
  pos = 1;
}

void TiltMotor::tiltNeutral(){
  if (pos==0) return;
  Serial.println("returning to neutral");
  message = "returned to neutral\n";
  if (pos>0) {
  attachInterrupt(digitalPinToInterrupt(S),finishTilt, FALLING);
  T.backward(speed*3/4);
  }
  else if (pos<0) {
  attachInterrupt(digitalPinToInterrupt(S),finishTilt, FALLING);
  T.forward(speed*3/4);
  }
  pos = 0;
}

void TiltMotor::tiltDown(){
  if (pos<0) return;
  Serial.println("tilting down");
  message = "tilted down\n";
  attachInterrupt(digitalPinToInterrupt(S),finishTilt, RISING);
  T.backward(speed);
  pos = -1;
}

void TiltMotor::finishHome(){
  detachInterrupt(digitalPinToInterrupt(S));
  T.brake();
  homed = true;
  Serial.write(message);
}

void TiltMotor::finishTilt(){
  detachInterrupt(digitalPinToInterrupt(S));
  T.brake();
  Serial.write(message);
}

