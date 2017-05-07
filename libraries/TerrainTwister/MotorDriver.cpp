#include <MotorDriver.h>

int MotorDriver::numC, MotorDriver::numCS;
MotorDriver* MotorDriver::currentQueue;
int* MotorDriver::analogQueue;
volatile int* MotorDriver::analogReadings;

void MotorDriver::init(){
	if (A>=0) pinMode(A,OUTPUT);
	if (B>=0) pinMode(B,OUTPUT);
	if (P>=0) pinMode(P,OUTPUT);
	if (CS>=0) pinMode(CS, INPUT);
	this->off();
}

void MotorDriver::init(int pinA, int pinB, int pinP){
	A = pinA; 
	B = pinB;
	P = pinP;
	init();
}


void MotorDriver::setSpeed(int speed){
	if (speed>0) forward(speed);
	else if (speed<0) backward(-speed);
	else off();
}

void MotorDriver::brake(bool high /*= false*/){
	digitalWrite(P,HIGH);
	digitalWrite(A,high);
	digitalWrite(B,high);
}


