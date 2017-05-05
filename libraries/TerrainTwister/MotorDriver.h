#ifndef _MOTORDRIVER_H
#define _MOTORDRIVER_H

#include <Arduino.h>

extern "C" void ADC_vect(void) __attribute__((signal));
extern "C" void TIMER2_OVF_vect(void) __attribute__((signal));

class MotorDriver{
	friend void ::ADC_vect(void);
	friend void ::TIMER2_OVF_vect(void);
	
	public:
		MotorDriver(int pinA, int pinB, int pwmPin) : A(pinA), B(pinB), P(pwmPin), CS(-1){};
		MotorDriver() : A(-1), B(-1), P(-1), CS(-1) {};
		void init();
		void init(int pinA, int pinB, int pinP );
		inline void MotorDriver::forward(int speed) {
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
		static uint8_t queueAnalogSense(uint8_t);
		static inline int readAnalogSense(uint8_t position) {return *(analogQueue+position)};
	private:
		int A, B, P, CS, Set;
		uint8_t aPos;
		bool CSenabled;
		static int numC, numCS;
		static MotorDriver *currentQueue;
		static int* analogQueue;
		static volatile int* analogReadings;
		static void queueCurrent();
};

#endif
