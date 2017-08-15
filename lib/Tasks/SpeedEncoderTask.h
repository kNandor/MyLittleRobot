#ifndef SPEED_ENCODER_TASK_HEADER
#define SPEED_ENCODER_TASK_HEADER
#include <TaskManager.h>
#include <SpeedEncoder.h>
#include <Filter.h>


class CSpeedEncoderFilterFnc{
public:
  virtual float getFilteredRotation()=0;
};
class CSpeedEncoderFilterTask:public CTask,public CSpeedEncoderFilterFnc{
public:
  CSpeedEncoderFilterTask(    uint32_t            f_period
                              ,CSpeedGetterFnc&    f_sensorGetterFnc
                              ,Filter::FIR::CFilterFnc&        f_filterFnc)
                              :CTask(f_period)
                              ,m_sensorGetterFnc(f_sensorGetterFnc)
                              ,m_filterFnc(f_filterFnc)
  {}
  void        _run(){
    CSpeedData l_data=m_sensorGetterFnc.getData();
    m_filteredValue=m_filterFnc.filtering(l_data.rps);
  }

  float      getFilteredRotation(){
    return m_filteredValue;
  }
private:
  CSpeedGetterFnc&             m_sensorGetterFnc;
  Filter::FIR::CFilterFnc&     m_filterFnc;
  float                             m_filteredValue;
};


#endif
