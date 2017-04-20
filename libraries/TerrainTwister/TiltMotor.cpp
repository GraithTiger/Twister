#include <TiltMotor.h>
#include <MotorDriver.h>

void TiltMotor::init() {
  T.init();
  pinMode(H, INPUT_PULLUP);
  pinMode(L, INPUT_PULLUP);
}

bool TiltMotor::home() {
  homed = false;
  if (digitalRead(H) && digitalRead(L)) {
    homed = true;
    pos = 0;
    return homed;
  }
  attachInterrupt(digitalPinToInterrupt(H),finishHome, RISING);
  attachInterrupt(digitalPinToInterrupt(L),finishHome, RISING);
  T.forward(speed); //try tilting up first;
  delay(3000);
  if (homed) {
    pos = 0;
    return homed;
  }
  T.backward(speed); //if not homed yet, try tilting down.
  delay(3000);
  if (homed) pos = 0;
  return homed;
}

void TiltMotor::tiltUp(){
  if (pos>0) return;
  attachInterrupt(digitalPinToInterrupt(H),finishTilt, FALLING);
  attachInterrupt(digitalPinToInterrupt(L),finishTilt, FALLING);
  T.forward(speed);
  pos = 1;
}

void TiltMotor::tiltNeutral(){
  if (pos==0) return;
  if (pos>0) {
  attachInterrupt(digitalPinToInterrupt(H),finishTilt, RISING);
  attachInterrupt(digitalPinToInterrupt(L),finishTilt, RISING);
  T.backward(speed);
  }
  if (pos>0) {
  attachInterrupt(digitalPinToInterrupt(H),finishTilt, RISING);
  attachInterrupt(digitalPinToInterrupt(L),finishTilt, RISING);
  T.forward(speed);
  }
  pos = 0;
}

void TiltMotor::tiltDown(){
  if (pos<0) return;
  attachInterrupt(digitalPinToInterrupt(H),finishTilt, FALLING);
  attachInterrupt(digitalPinToInterrupt(L),finishTilt, FALLING);
  T.backward(speed);
  pos = -1;
}

void TiltMotor::finishHome(){
  detachInterrupt(digitalPinToInterrupt(tilt->H));
  detachInterrupt(digitalPinToInterrupt(tilt->L));
  tilt->T.brake();
  tilt->homed = true;
}

void TiltMotor::finishTilt(){
  detachInterrupt(digitalPinToInterrupt(tilt->H));
  detachInterrupt(digitalPinToInterrupt(tilt->L));
  tilt->T.brake();
}

