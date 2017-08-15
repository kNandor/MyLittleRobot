#ifndef INCREMENTAL_ENCODER_H
#define INCREMENTAL_ENCODER_H


#include<Arduino.h>

class CInterruptCounter{
  public:
    CInterruptCounter(byte f_pin,void (*f_staticIncrement)(void))
    :m_pin(f_pin)
    ,m_count(0)
    {
      attachInterrupt(digitalPinToInterrupt(f_pin),f_staticIncrement,CHANGE);
    }

    void increment(){
      m_count++;
    }

    void reset(){
      m_count=0;
    }

    uint64_t getCount(){
      return m_count;
    }

  private:
    const byte m_pin;
    volatile uint64_t m_count;
};


#endif
