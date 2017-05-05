#ifndef _MOTORDRIVER_H
#define _MOTORDRIVER_H

#include <Arduino.h>

class MotorDriver{
	int A, B, P, CS;

	public:
		MotorDriver(int pinA, int pinB, int pwmPin) : A(pinA), B(pinB), P(pwmPin), CS(-1){};
		MotorDriver() : A(-1), B(-1), P(-1), CS(-1) {};
		void init();
		void init(int pinA, int pinB, int pinP );
		inline void forward(int speed) {
			analogWrite(P,speed);
			digitalWrite(A,HIGH);
			digitalWrite(B,LOW);
		};

		inline void backward(int speed){
			analogWrite(P,speed);
			digitalWrite(B,HIGH);
			digitalWrite(A,LOW);
		};
		inline void off(){
			digitalWrite(P,LOW);
			digitalWrite(A,LOW);
			digitalWrite(B,LOW);
		}
		void setSpeed(int);
		void brake(bool high = false);
};

#endif
