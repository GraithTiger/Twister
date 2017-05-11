#ifndef _TILTMOTOR_H
#define _TILTMOTOR_H

#include <Arduino.h>
#include <MotorDriver.h>

class TiltMotor{
  public:
    TiltMotor(int pinA, int pinB, int pinP, int sense) : pos(0), A(pinA), B(pinB), P(pinP) { S = sense; };
    void init();
    bool home();
    void tiltUp();
    void tiltNeutral();
    void tiltDown();
    bool curPos() {return pos;};
    static void finishTilt();
    static void finishHome();
    static TiltMotor* spthis = NULL;
  private:
    int pos;
    const int speed = 255;
    static volatile bool homed;
    //static char* message;
    static int S;
	int A, B, P;
    static MotorDriver T;
};

#endif

