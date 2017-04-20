#include <TerrainTwister.h>

//pin definitions
#define leftA 2
#define leftB 4
#define leftP 9
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

MotorDriver left = MotorDriver(leftA,leftB,leftP);
MotorDriver right = MotorDriver(rightA,rightB,rightP);
//TiltMotor::tilt = new TiltMotor(tiltA,tiltB,tiltP,tiltHi,tiltLo);

void setup(){
  //TiltMotor::tilt.init();
  left.init();
  right.init();
  delay(1000);
  //TiltMotor::tilt.home();
}

void loop(){
  //Go forward for five seconds
  left.forward(255);
  right.forward(255);
  /*delay(5000);
  
  //brake and tilt up
  left.brake();
  right.brake();
  //TiltMotor::tilt.tiltUp();
  delay(1000);

  //roll sideways for five seconds
  left.forward(64);
  right.backward(64);
  delay(5000);

  //brake and tilt down
  left.brake();
  right.brake();
  //TiltMotor::tilt.tiltDown();
  delay(1000);

  //roll the other way for five seconds
  left.backward(64);
  right.forward(64);
  delay(5000);

  //brake and level tilt
  left.brake();
  right.brake();
  //TiltMotor::tilt.tiltNeutral();
  delay(1000);

  //Go backward for five seconds
  left.backward(1);
  right.backward(127);
  delay(5000);

  //Pause for 10 seconds
  left.brake();
  right.brake();
  delay(10000);*/
}

