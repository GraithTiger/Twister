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

MotorDriver leftScrew = MotorDriver(leftA,leftB,leftP);
MotorDriver rightScrew = MotorDriver(rightA,rightB,rightP);
//TiltMotor::tilt = new TiltMotor(tiltA,tiltB,tiltP,tiltHi,tiltLo);


void setup()
{ /*WARNING*/
  //Shutdown pins of VL53L0X ACTIVE-LOW-ONLY NO TOLERANT TO 5V will fry them
  pinMode(XSHUT_pin1, OUTPUT);
  pinMode(XSHUT_pin2, OUTPUT);
  /*pinMode(XSHUT_pin3, OUTPUT);
  pinMode(XSHUT_pin4, OUTPUT);
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
  Sensor4.setAddress(Sensor4_newAddress);
  pinMode(XSHUT_pin3, INPUT);
  delay(10);
  Sensor3.setAddress(Sensor3_newAddress);*/
  pinMode(XSHUT_pin2, INPUT);
  delay(10);
  Sensor2.setAddress(Sensor2_newAddress);
  pinMode(XSHUT_pin1, INPUT);
  delay(10);
  
  Sensor1.init();
  Sensor2.init();
  /*Sensor3.init();
  Sensor4.init();
  Sensor5.init();
  Sensor6.init();*/
  
  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);
  /*Sensor3.setTimeout(500);
  Sensor4.setTimeout(500);
  Sensor5.setTimeout(500);
  Sensor6.setTimeout(500);*/

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  Sensor1.startContinuous();
  Sensor2.startContinuous();
  /*Sensor3.startContinuous();
  Sensor4.startContinuous();
  Sensor5.startContinuous();
  Sensor6.startContinuous();*/

  //Motor Stuff
  leftScrew.init();
  rightScrew.init();
  //leftservo.attach(10);
  //rightservo.attach(9);

}

void loop()
{
  
  int left = Sensor1.readRangeContinuousMillimeters();
  int right = Sensor2.readRangeContinuousMillimeters();
  Serial.print(left);
  Serial.print(',');
 
  Serial.print(right);
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
  int ss_speed = 0;
  int position_diff = left - right;
  int Kp = 3;
  int correction = ss_speed + position_diff/Kp;
  if (abs(position_diff) < 500){
	leftScrew.setSpeed(correction);
	rightScrew.setSpeed(-correction);
  }
  /*
  if (position_diff > 0)
  {
    rightservo.write(94+ correction);
    leftservo.write(95- ss_speed);
  }
  else if (position_diff < 0)
  {
    rightservo.write(94- ss_speed);
    leftservo.write(95+ correction); 
  }
  else
  {
    rightservo.write(94- ss_speed);
    leftservo.write(95+ ss_speed);
  }  
  */
}













