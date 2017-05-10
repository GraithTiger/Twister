#ifndef _MOTORDRIVER_H
#define _MOTORDRIVER_H

#include <Arduino.h>

extern "C" void ADC_vect(void) __attribute__((signal));
extern "C" void TIMER2_OVF_vect(void) __attribute__((signal));

class MotorDriver{
	friend void ::ADC_vect(void);
	friend void ::TIMER2_OVF_vect(void);
	
	public:
		MotorDriver(int pinA, int pinB, int pwmPin) :
			A(pinA), B(pinB), P(pwmPin), CS(-1), HiFreq(false),
			aPos(-1), CCenabled(false), Set(0), Kp(10) {};
		MotorDriver(int pinA, int pinB, int pwmPin, int CSpin) :
			A(pinA), B(pinB), P(pwmPin), CS(CSpin), HiFreq(false),
			aPos(-1), CCenabled(false), Set(0), Kp(10)  {};
		MotorDriver() :
			A(-1), B(-1), P(-1), CS(-1), HiFreq(false),
			aPos(-1), CCenabled(false), Set(0), Kp(10)  {};
		void init();
		void init(bool);
		void init(int pinA, int pinB, int pinP );
		void setSpeed(int);
		void setCurrent(long);
		void brake(bool high = false);
		inline int getKp() { return Kp;}
		inline void setKp( int k ) {Kp = k;}
		static uint8_t queueAnalogSense(uint8_t);
		static inline int readAnalogSense(uint8_t position) {
			return *(analogQueue+position);
		}
	private:
		int A, B, P, CS, Set, Kp;
		uint8_t aPos;
		bool CCenabled, HiFreq;
		static int numA, numCC;
		static MotorDriver** currentQueue;
		static uint8_t* analogQueue;
		static volatile int adcPos;
		static volatile int* analogReadings;
		void initCurrent();
		inline void MotorDriver::forward(int speed) {
			speed = constrain(speed,0,400);
			if (HiFreq && P==9) OCR1A = speed;
			else if (HiFreq && P==10) OCR1B = speed;
			else analogWrite(P,speed*51/80);
			digitalWrite(A,HIGH);
			digitalWrite(B,LOW);
		};

		inline void backward(int speed){
			speed = constrain(speed,0,400);
			if (HiFreq && P==9) OCR1A = speed;
			else if (HiFreq && P==10) OCR1B = speed;
			else analogWrite(P,speed*51/80);
			digitalWrite(B,HIGH);
			digitalWrite(A,LOW);
		};
		inline void off(){
			if (HiFreq && P==9) OCR1A = 0;
			else if (HiFreq && P==10) OCR1B = 0;
			else analogWrite(P,0);
			digitalWrite(A,LOW);
			digitalWrite(B,LOW);
		}
		static inline void enableADRI(uint8_t pin)
		{
			if (pin>14) {
				#if defined(__AVR_ATmega328P__)
					pin -= 14;
				#endif
			}
			// clear ADLAR in ADMUX (0x7C) to right-adjust the result
			// ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
			ADMUX &= B11011111;
			// Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
			// proper source (01)
			ADMUX |= B01000000;
			// Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
			// input
			ADMUX &= B11110000;
			// Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
			// Do not set above 15! You will overrun other parts of ADMUX. A full
			// list of possible inputs is available in Table 24-4 of the ATMega328
			// datasheet
			ADMUX |= (pin & 0x07);
			// ADMUX |= B00001000; // Binary equivalent
			// Set ADEN in ADCSRA (0x7A) to enable the ADC.
			// Note, this instruction takes 12 ADC clocks to execute
			ADCSRA |= B10000000;
			// Clear ADATE in ADCSRA (0x7A) to disable auto-triggering.
			ADCSRA &= ~B00100000;
			// Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
			// This means that as soon as an ADC has finished, the next will be
			// immediately started.
			//ADCSRB &= B11111000;
			// Set the Prescaler to 128 (16000KHz/128 = 125KHz)
			// Above 200KHz 10-bit results are not reliable.
			ADCSRA |= B00000111;
			// Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
			// Without this, the internal interrupt will not trigger.
			ADCSRA |= B00001000;
			// Kick off the first ADC
			// Set ADSC in ADCSRA (0x7A) to start the ADC conversion
			ADCSRA |=B01000000;
		}
};

#endif
