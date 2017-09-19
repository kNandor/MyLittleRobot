#ifndef TASK_MAMANGER_HEADER
#define TASK_MAMANGER_HEADER

#include <stdint.h>

class CTask{
public:

  CTask(  uint32_t        f_period)
  :m_period(f_period)
  {
    m_step=0;
  }
  virtual void _run()=0;

  void checkTask(){
    ++m_step;
    if(m_step==m_period-1){
      this->_run();
    }else if(m_step>=m_period){
      m_step=0;
    }
  }
private:
  const uint32_t        m_period;
  uint32_t              m_step;

};


class CTaskManager{
public:
  CTaskManager( const float               f_baseTick
                ,uint32_t                 f_nrTask
                ,CTask**                  f_tasks)
                :m_baseTick(static_cast<uint32_t>(f_baseTick*1e6))
                ,m_nrTask(f_nrTask)
                ,m_tasks(f_tasks)
  {
    // timer2.setup();
    m_timestamp=micros();
  }

  void run(){
    uint64_t l_timestamp_us=micros();
    if(m_timestamp>l_timestamp_us){
      m_timestamp=0;//millis restart
    }
    if(l_timestamp_us-m_timestamp>=m_baseTick){
      m_timestamp=l_timestamp_us;
      for(uint32_t i=0;i<m_nrTask;++i){
        m_tasks[i]->checkTask();
      }
    }
  }
private:
  // const float       m_baseTick;
  const uint32_t    m_baseTick;
  uint32_t          m_timestamp;
  const uint32_t    m_nrTask;
  CTask**           m_tasks;

};
#endif
