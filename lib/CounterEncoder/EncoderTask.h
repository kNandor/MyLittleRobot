#ifndef ENCODER_TASK_H
#define ENCODER_TASK_H

#include <TaskManager.h>
#include <IncrementalEncoder.h>

class CEncoderTask:public CTask{
public:
  CEncoderTask( uint32_t                f_period
                ,float                  f_period_s
                ,uint32_t               f_cpr
                ,CInterruptCounter&     f_counter)
                :CTask(f_period)
                ,m_period_s(f_period_s)
                ,m_cpr(f_cpr)
                ,m_counter(f_counter)
                ,m_rps(0)
  {}

  void _run(){
    uint64_t l_count=m_counter.getCount();
    m_counter.reset();
    m_rps=((float)l_count)/m_period_s/16.0/m_cpr;
  }

  float getRps(){return m_rps;}
private:
  const float               m_period_s;
  const uint32_t            m_cpr;
  CInterruptCounter&        m_counter;
  float                     m_rps;
};

#endif
