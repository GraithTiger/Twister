#ifndef _TILTMOTOR_H
#define _TILTMOTOR_H

#include <Arduino.h>
#include <MotorDriver.h>

class TiltMotor{
  public:
    TiltMotor();
    TiltMotor(int A, int B, int P, int Hi, int Lo): T(A,B,P), H(Hi), L(Lo) {};
    void init();
    bool home();
    void tiltUp();
    void tiltNeutral();
    void tiltDown();
    bool curPos() {return pos;};
    static void finishTilt();
    static void finishHome();
    static TiltMotor *tilt = NULL;
  private:
    int pos;
    const int speed = 127;
    volatile bool homed;
    int H, L;
    MotorDriver T;
};

#endif

