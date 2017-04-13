#include <Arduino.h>

#ifndef _MOTORDRIVER_H
#define _MOTORDRIVER_H


class MotorDriver{
  int A, B, P;
  public:
    MotorDriver(int pinA, int pinB, int pwmPin) : A(pinA), B(pinB), P(pwmPin) {}
    void init();
    void forward(int speed);
    void backward(int speed);
    void off();
    void brake(bool high = false);
};

#endif
