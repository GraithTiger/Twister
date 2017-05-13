#include "Arduino.h"
#include <TerrainTwister.h>
//The setup function is called once at startup of the sketch

//pin definitions
#define leftA 2
#define leftB 4
#define leftP 9
#define leftCS 0

#define rightA 7
#define rightB 8
#define rightP 10
#define rightCS 1

#define tiltA 11
#define tiltB 13
#define tiltP 5
#define tiltSense 3

MotorDriver leftScrew = MotorDriver(leftA,leftB,leftP,leftCS);
MotorDriver rightScrew = MotorDriver(rightA,rightB,rightP,rightCS);
TiltMotor tilt = TiltMotor(tiltA,tiltB,tiltP,tiltSense);


void setup()
{
// Add your initialization code here
	Serial.begin(9600);
	rightScrew.init(true);
	delay(1000);
	rightScrew.setKp(5);
	rightScrew.setCurrent(2000);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	Serial.println(MotorDriver::readAnalogSense(0)*34);
	delay(100);
}
