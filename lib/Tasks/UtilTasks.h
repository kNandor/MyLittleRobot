#ifndef UTIL_TASKS_HEADER
#define UTIL_TASKS_HEADER

#include <Arduino.h>
#include <SpeedEncoder.h>
#include <TaskManager.h>
#include <QueueArray.h>
#include <string.h>
#include <stdarg.h>
// #include <cstdio.h>


// class CSpeedEncoderT: public CTask{
// public:
//   CSpeedEncoder(uint8_t     f_SensorPin,
//                 uint8_t     f_period,
//                 float       f_period_s)
//                 :CTask(f_period)
//                 ,m_SensorPin(f_SensorPin)
//                 ,m_period_s(f_period_s)
//   {}
//
//   void _run(){
//
//   }
// private:
//   const uint8_t       m_SensorPin;
//   const float         m_period_s;
// };
class CSpeedEncoderReadTask:public CTask,public CSpeedGetterFnc{
public:
  CSpeedEncoderReadTask(  uint32_t        f_period
                          ,float          f_period_s
                          ,uint8_t        f_pin
                          ,uint32_t       f_ppr)
  :CTask(f_period)
  ,m_period_s(f_period_s)
  ,m_numarator(f_pin)
  ,m_ppr(f_ppr)
  {
    m_data.timestamp_us=micros();
    m_data.rps=0.0;
  }

  void _run(){
    uint32_t l_nr=m_numarator.getNrSemnal();
    m_numarator.reset();
    //float l_rot2=l_nrSem/40.0/((m_interval_us/1.e6)/60)/15.5;
    float l_rps=(static_cast<float>(l_nr)/m_ppr)/(m_period_s/60.0)/15.5;
    uint32_t l_timestamp_us=micros();
    m_data.rps=l_rps;
    m_data.timestamp_us=l_timestamp_us;
  }

  CSpeedData            getData(){return m_data;}
  const CNumarator&     getNumarator(){return m_numarator;}

private:
  const float           m_period_s;
  CNumarator            m_numarator;
  const uint32_t        m_ppr;
  CSpeedData            m_data;
};


class CBlinker: public CTask{
public:
  CBlinker( uint8_t         f_ledPin
            ,uint32_t       f_period)
            :CTask(f_period)
            ,m_ledPin(f_ledPin)
            ,m_ledState(true)
  {}
  void _run(){
    digitalWrite(m_ledPin,m_ledState?HIGH:LOW);
    m_ledState=!m_ledState;
  }

private:
  const uint8_t             m_ledPin;
  bool                      m_ledState;
};


class CSerialHandler:public CTask{
public:
  CSerialHandler( uint32_t                 f_period
                  ,HardwareSerial&         f_hardwareSerial)
                  :CTask(f_period)
                  ,m_hardwareSerial(f_hardwareSerial)
                  ,m_buffer()
  {
    m_buffer.setPrinter(f_hardwareSerial);
  }
  void            _run(){
    // m_hardwareSerial.println("");
    if(!m_buffer.isEmpty()){
      m_hardwareSerial.print(m_buffer.dequeue());
    }
  }
  int             append(const char *format,...){
    char l_buffer[200];
    va_list varg;
    va_start(varg,format);
    uint8_t n=vsnprintf(l_buffer,200,format,varg);
    appendStr(l_buffer);
    va_end(varg);
    return n;
  }

  void            appendStr( String                f_string){
    for(uint16_t i=0;i<f_string.length();++i){
      m_buffer.enqueue(f_string[i]);
    }
  }
  private:
  HardwareSerial&             m_hardwareSerial;
  QueueArray<char>            m_buffer;
};



#endif
