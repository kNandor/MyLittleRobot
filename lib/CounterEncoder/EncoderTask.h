#ifndef ENCODER_TASK_H
#define ENCODER_TASK_H

#include <TaskManager.h>
#include <IncrementalEncoder.h>


typedef struct{
  uint32_t    timestamp_us;
  float       rps;
}CSpeedData;

class CSpeedGetterFnc{
public:
  virtual CSpeedData getData()=0;
};

class CEncoderTask:public CTask,public CSpeedGetterFnc{
public:
  CEncoderTask( uint32_t                f_period
                ,float                  f_period_s
                ,uint32_t               f_cpr
                ,CInterruptCounter&     f_counter)
                :CTask(f_period)
                ,m_period_s(f_period_s)
                ,m_cpr(f_cpr)
                ,m_counter(f_counter)
                // ,m_rps(0)
  {
    m_data.timestamp_us=micros();
    m_data.rps=0;
  }

  void _run(){
    uint64_t l_count=m_counter.getCount();
    m_counter.reset();
    uint32_t timestamp=micros();
    // m_rps=((float)l_count)/m_period_s/16.0/m_cpr;
    m_data.rps=((float)l_count)/m_period_s/16.0/m_cpr;
    m_data.timestamp_us=timestamp;
  }

  virtual CSpeedData getData(){
    return m_data;
  }

  float getRps(){return m_data.rps;}
private:
  const float               m_period_s;
  const uint32_t            m_cpr;
  CInterruptCounter&        m_counter;
  // float                     m_rps;
  CSpeedData                m_data;
};

#endif
