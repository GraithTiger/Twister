//Motor Driver Test Suite
#include <MotorDriver.h>

#define INA 9
#define INB 10
#define PWM 13

MotorDriver testmotor = MotorDriver(INA, INB, PWM);

void setup() {
  testmotor.init();
}

void loop() {
  for (int i=0;i<256;i++) {
    testmotor.forward(i);
    delay(40);
  }
  testmotor.off();
  delay(5000);
  for (int i=0;i<256;i++) {
    testmotor.backward(i);
    delay(40);
  }
  testmotor.brake();
  delay(2500);
  testmotor.forward(255);
  delay(5000);
  testmotor.brake(true);
  delay(10000);
}

