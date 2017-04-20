#include <MotorDriver.h>


void MotorDriver::init(){
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(P,OUTPUT);
  this->off();
}

void MotorDriver::forward(int speed) {
  analogWrite(P,speed);
  digitalWrite(A,HIGH);
  digitalWrite(B,LOW);
}

void MotorDriver::backward(int speed){
  analogWrite(P,speed);
  digitalWrite(B,HIGH);
  digitalWrite(A,LOW);
}

void MotorDriver::off(){
  digitalWrite(P,LOW);
  digitalWrite(A,LOW);
  digitalWrite(B,LOW);
}

void MotorDriver::brake(bool high /*= false*/){
  digitalWrite(P,HIGH);
  digitalWrite(A,high);
  digitalWrite(B,high);
}


