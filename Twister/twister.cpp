#include "twister.h"
#include <Wire.h>
#include <TerrainTwister.h>
#include <VL53L0X.h>

//#define XSHUT_pin6 not required for address change
//#define XSHUT_pin5 9
//#define XSHUT_pin4 8
#define XSHUT_pin3 A2
#define XSHUT_pin2 6
#define XSHUT_pin1 12

//ADDRESS_DEFAULT 0b0101001 or 41
//#define Sensor1_newAddress 41 not required address change
#define Sensor2_newAddress 42
#define Sensor3_newAddress 43
#define Sensor4_newAddress 44

#define Sensor5_newAddress 45
#define Sensor6_newAddress 46

VL53L0X Sensor1;
VL53L0X Sensor2;
VL53L0X Sensor3;
/*VL53L0X Sensor4;
VL53L0X Sensor5;
VL53L0X Sensor6;*/

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

MotorDriver leftScrew = MotorDriver(leftA,leftB,leftP);
MotorDriver rightScrew = MotorDriver(rightA,rightB,rightP);
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
  leftScrew.init();
  rightScrew.init();
  tilt.init();
  tilt.home();
}

void loop()
{
  
  int left = Sensor1.readRangeContinuousMillimeters();
  int right = Sensor2.readRangeContinuousMillimeters();
  Serial.print(left);
  Serial.print(',');
  Serial.print(right);
  //Serial.println();
  Serial.print(',');
  Serial.println(Sensor3.readRangeContinuousMillimeters());

  //P control loop for centering
  int ss_speed = 0;
  int position_diff = left - right;
  int Kp = 3;
  int correction = ss_speed + position_diff/Kp;
  if (abs(position_diff) < 500){
	leftScrew.setSpeed(correction);
	rightScrew.setSpeed(-correction);
  }
}













