#ifndef UTIL_TASKS_HEADER
#define UTIL_TASKS_HEADER

#include <Arduino.h>
#include <SoftwareSerial.h>
// #include <SpeedEncoder.h>
#include <TaskManager.h>
#include <QueueArray.h>
#include <string.h>
#include <stdarg.h>
// #include <cstdio.h>



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

class CRunTester: public CTask{
public:
  CRunTester  (uint32_t    f_period
              ,float    f_period_s)
              :CTask(f_period)
              ,m_period_us(f_period_s*1e6)
              {
                m_timestamp_us=micros();
                m_duration=0.0;
              }
  void _run(){
    uint32_t l_timestamp_us=micros();
    m_duration=l_timestamp_us-m_timestamp_us;
    m_timestamp_us=l_timestamp_us;
    if(m_duration>1.5*m_period_us){
      m_error++;
    }
  }

  uint32_t getDuration(){
    return m_duration;
  }

  uint32_t    get(){
    return m_error;
  }
  uint32_t    getD(){
    return m_period_us;
  }
  void        reset(){
    m_error=0;
  }
private:
  uint32_t              m_timestamp_us;
  uint32_t              m_duration;
  uint32_t              m_error;
  const uint32_t        m_period_us;
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
