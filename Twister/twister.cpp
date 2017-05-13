#include "twister.h"
#include <Wire.h>
#include <TerrainTwister.h>
#include <VL53L0X.h>
#include <Servo.h>

//#define XSHUT_pin6 not required for address change
#define XSHUT_pin5 9
#define XSHUT_pin4 8
#define XSHUT_pin3 7
#define XSHUT_pin2 5
#define XSHUT_pin1 3

//ADDRESS_DEFAULT 0b0101001 or 41
//#define Sensor1_newAddress 41 not required address change
#define Sensor2_newAddress 42
#define Sensor3_newAddress 43
#define Sensor4_newAddress 44

#define Sensor5_newAddress 45
#define Sensor6_newAddress 46

VL53L0X Sensor1;
VL53L0X Sensor2;
/*VL53L0X Sensor3;
VL53L0X Sensor4;
VL53L0X Sensor5;
VL53L0X Sensor6;

Servo leftservo;
Servo rightservo;*/

//pin definitions
#define leftA 2
#define leftB 4
#define leftP 9
#define leftCS A0
#define leftEnable 6

#define rightA 7
#define rightB 8
#define rightP 10
#define rightEnable 12

#define tiltA A2
#define tiltB A3
#define tiltP 11
#define tiltHi 2
#define tiltLo 3

MotorDriver leftScrew = MotorDriver(leftA,leftB,leftP,leftCS);
MotorDriver rightScrew = MotorDriver(rightA,rightB,rightP,rightCS);
TiltMotor tilt = TiltMotor(tiltA,tiltB,tiltP,tiltSense);


void setup()
{ /*WARNING*/
  //Shutdown pins of VL53L0X ACTIVE-LOW-ONLY NO TOLERANT TO 5V will fry them
  pinMode(XSHUT_pin1, OUTPUT);
  pinMode(XSHUT_pin2, OUTPUT);
  pinMode(XSHUT_pin3, OUTPUT);
  /*pinMode(XSHUT_pin4, OUTPUT);
  pinMode(XSHUT_pin5, OUTPUT);*/
  
  Serial.begin(9600);
  
  Wire.begin();
  //Change address of sensor and power up next one
  /*Sensor6.setAddress(Sensor6_newAddress);
  pinMode(XSHUT_pin5, INPUT);
  delay(10); //For power-up procedure t-boot max 1.2ms "Datasheet: 2.9 Power sequence"
  Sensor5.setAddress(Sensor5_newAddress);
  pinMode(XSHUT_pin4, INPUT);
  delay(10);
  Sensor4.setAddress(Sensor4_newAddress);*/
  pinMode(XSHUT_pin3, INPUT);
  delay(10);
  Sensor3.setAddress(Sensor3_newAddress);
  pinMode(XSHUT_pin2, INPUT);
  delay(10);
  Sensor2.setAddress(Sensor2_newAddress);
  pinMode(XSHUT_pin1, INPUT);
  delay(10);
  
  Sensor1.init();
  Sensor2.init();
  Sensor3.init();
  /*Sensor4.init();
  Sensor5.init();
  Sensor6.init();*/
  
  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);
  Sensor3.setTimeout(500);
  /*Sensor4.setTimeout(500);
  Sensor5.setTimeout(500);
  Sensor6.setTimeout(500);*/

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  Sensor1.startContinuous();
  Sensor2.startContinuous();
  Sensor3.startContinuous();
  /*Sensor4.startContinuous();
  Sensor5.startContinuous();
  Sensor6.startContinuous();*/

  //Motor Stuff
  leftScrew.init(true);
  rightScrew.init(true);
  tilt.init();
  tilt.home();
}

void loop() {
	int left = Sensor1.readRangeContinuousMillimeters();
	int right = Sensor2.readRangeContinuousMillimeters();
	int front = Sensor3.readRangeContinuousMillimeters();
	Serial.print(left);
	Serial.print(',');
	Serial.print(right);
	Serial.print(',');
	Serial.print(front);
	Serial.println();
	/*
	Base code hold over
	Serial.print(Sensor1.readRangeContinuousMillimeters());
	Serial.print(',');
	Serial.print(Sensor2.readRangeContinuousMillimeters());
	Serial.println();
	/*
	//Serial.print(Sensor3.readRangeContinuousMillimeters());
	//Serial.print(','); Did not work for me result was 65535
	Serial.print(Sensor4.readRangeContinuousMillimeters());
	Serial.print(',');
	Serial.print(Sensor5.readRangeContinuousMillimeters());
	Serial.print(',');
	Serial.print(Sensor6.readRangeContinuousMillimeters());
	Serial.println();
	*/
	/*
	int ss_speed = 0;
	int position_diff = left - right;
	int Kp = 5;
	int correction = ss_speed + position_diff/Kp;
	int position_ratio = left/right;
	int position_ratio_cushion_r = 10;
	int position_ratio_cushion_l = 1/position_ratio_cushion_r;
	int front_proximity = 200;
	int cornering_cushion = 2*front_proximity;
	int cornering_speed = ss_speed/2;
	// how is the speed defined - check the spin orientation.
	//Pipe End
		if (left <1000 && right <1000 && front<1000){
			//Correcting Right
			if (position_ratio < position_ratio_cushion_l){
				leftScrew.setSpeed(correction);
				rightScrew.setSpeed(ss_speed);
			}
			//Correcting Left
			if (position_ratio > position_ratio_cushion_r){
				leftScrew.setSpeed(ss_speed);
				rightScrew.setSpeed(correction);
			}
			// Wall Avoidance
			if (front < front_proximity){
				leftScrew.setSpeed(0);
				rightScrew.setSpeed(0);
				// Cornering
				if(front < cornering_cushion){
					leftScrew.setSpeed(cornering_speed);
					rightScrew.setSpeed(-cornering_speed);
				}
			}
		}
		else{
			leftScrew.setSpeed(0);
			rightScrew.setSpeed(0);
		}
	*/
	int ss_speed = 2000;
	int position_diff = left - right;
	int Kp = 3;
	int correction = 2000*(position_diff/abs(position_diff)); //position_diff*Kp;
	//Serial.println(correction);
	if (abs(position_diff) < 500 || right<200 || left<200){
		leftScrew.setCurrent(ss_speed-correction);
		rightScrew.setCurrent(ss_speed+correction);
		Serial.print(ss_speed-correction); Serial.print(','); Serial.println(ss_speed+correction);
	}
	else{
		leftScrew.setCurrent(ss_speed);
		rightScrew.setCurrent(ss_speed);
	}
}













