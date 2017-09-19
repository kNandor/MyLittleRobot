#ifndef MOTOR_DRIVER_HEADER
#define MOTOR_DRIVER_HEADER
#include <Arduino.h>
#include <stdint.h>

// This class is a motor driver, which control a continouos motor
// by using one PWM pin and two digital pin.
// The PWM can vary between 0 and 255.
// The  PinA and PinB determine the rotation orientation of the motor.
//    10-> rotates forward
//    01-> rotates backward

class CMotorDriver{
public:
  CMotorDriver(   uint8_t           f_pinPWM
                  ,uint8_t          f_pinA
                  ,uint8_t          f_pinB)
                  :m_pinPWM(f_pinPWM)
                  ,m_pinA(f_pinA)
                  ,m_pinB(f_pinB)
  {
    pinMode(f_pinPWM,OUTPUT);
    pinMode(f_pinA,OUTPUT);
    pinMode(f_pinB,OUTPUT);
  }
  void setMotorPWMForward( uint8_t         f_speedPWM){
    f_speedPWM=f_speedPWM%256;
    digitalWrite(m_pinA,HIGH);
    digitalWrite(m_pinB,LOW);
    analogWrite(m_pinPWM,f_speedPWM);
  }
  void setMotorPWMBack(    uint8_t         f_speedPWM){
    f_speedPWM=f_speedPWM%256;
    digitalWrite(m_pinA,LOW);
    digitalWrite(m_pinB,HIGH);
    analogWrite(m_pinPWM,f_speedPWM);
  }
  void setBraking(        uint8_t         f_speedPWM){
    f_speedPWM=f_speedPWM%256;
    digitalWrite(m_pinA,LOW);
    digitalWrite(m_pinB,LOW);
    analogWrite(m_pinPWM,f_speedPWM);
  }
private:
  const uint8_t m_pinPWM;
  const uint8_t m_pinA;
  const uint8_t m_pinB;
};

#endif
