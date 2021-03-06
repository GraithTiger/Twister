#include <MotorDriver.h>

int MotorDriver::numA = 0;
int MotorDriver::numCC = 0;
MotorDriver** MotorDriver::currentQueue;
uint8_t* MotorDriver::analogQueue;
volatile int* MotorDriver::analogReadings;
volatile int MotorDriver::adcPos;

void MotorDriver::init(int pinA, int pinB, int pinP){
	A = pinA;
	B = pinB;
	P = pinP;
	init();
}


//High Frequency Setup for Timer1 on Uno
void MotorDriver::init(bool highFreq) {
	#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
		if (highFreq && (P==9 || P==10)) {
			// Timer 1 configuration
			// prescaler: clockI/O / 1
			// outputs enabled
			// phase-correct PWM
			// top of 400
			//
			// PWM frequency calculation
			// 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
			TCCR1A = 0b10100000;
			TCCR1B = 0b00010001;
			ICR1 = 400;
			HiFreq = true;
		}
	#endif
	init();
}

void MotorDriver::init(){
	if (A>=0) pinMode(A,OUTPUT);
	if (B>=0) pinMode(B,OUTPUT);
	if (P>=0) pinMode(P,OUTPUT);
	if (CS>=0) {
		if (CS>14) {
			pinMode(CS, INPUT);
			#if defined(__AVR_ATmega328P__)
				CS -= 14;
			#endif
		}
		initCurrent();
	}
	this->off();
}

void MotorDriver::initCurrent(){
	aPos = queueAnalogSense(CS);
	if (numCC==0) {
		//setup timer2 interrupt here
		TIFR2 &= B11111110; //
		TIMSK2 |= B00000001; //set Timer 2 Interrupt enable
		currentQueue = new MotorDriver*[1];
		currentQueue[0] = this;
		numCC++;
	}
	else {
		MotorDriver** newQueue = new MotorDriver*[numCC+1];
		for (int i = 0; i<numCC; i++) {
			newQueue[i] = currentQueue[i];
		}
		newQueue[numCC] = this;
		numCC++;
		delete [] currentQueue;
		currentQueue = newQueue;
	}
}

uint8_t MotorDriver::queueAnalogSense(uint8_t pin){
	if (pin>14) {
		pinMode(pin, INPUT);
		#if defined(__AVR_ATmega328P__)
			pin -= 14;
		#endif
	}
	if (numA == 0) {
		analogQueue = new uint8_t[1];
		analogQueue[1] = pin;
		analogReadings = new int[1];
		analogReadings[1] = 0;
		adcPos = 0;
		numA++;
		enableADRI(pin);
	}
	else {
		uint8_t* newQueue = new uint8_t[numA+1];
		int* newReadings = new int[numA+1];
		for (int i=0; i<numA; i++) {
			newQueue[i] = analogQueue[i];
			newReadings[i] = analogReadings[i];
		}
		newQueue[numA] = pin;
		newReadings[numA] = 0;
		numA++;
		delete [] analogQueue;
		delete [] analogReadings;
		analogQueue = newQueue;
		analogReadings = newReadings;
	}
	return numA;
}


void MotorDriver::setSpeed(int speed){
	CCenabled = false;
	if (speed>0) forward(speed);
	else if (speed<0) backward(-speed);
	else off();
}

void MotorDriver::setCurrent(long mA){
	Set = mA/34;
	Serial.println(Set);
	CCenabled = true;
}

void MotorDriver::brake(bool high /*= false*/){
	CCenabled = false;
	if (HiFreq && P==9) OCR1A = 400;
	else if (HiFreq && P==10) OCR1B = 400;
	else analogWrite(P,255);
	digitalWrite(A,high);
	digitalWrite(B,high);
}

//analog read loop
ISR(ADC_vect) {
	int reading = ADCL | (ADCH << 8);
	MotorDriver::analogReadings[MotorDriver::adcPos] =  reading;//store reading
	MotorDriver::adcPos++;
	if (MotorDriver::adcPos>MotorDriver::numA) MotorDriver::adcPos=0;
	ADMUX |= (MotorDriver::analogQueue[1] & 0x07); //go to next pin to read
	ADCSRA |=B01000000; // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
}

//current control loop
ISR(TIMER2_OVF_vect) {
	for (int i = 0; i<MotorDriver::numCC; i++) {
		MotorDriver* c = MotorDriver::currentQueue[i];
		if (c->CCenabled) {
			if (c->Set==0)  c->off();
			else if (c->Set>0) {
				int val = (c->Kp) * ((c->Set) - MotorDriver::readAnalogSense(c->aPos));
				c->forward(val);
			}
			else {
				int val = (c->Kp) * ((c->Set) - MotorDriver::readAnalogSense(c->aPos));
				c->backward(val);
			}
		}
	}
}
