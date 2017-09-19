#ifndef MPU_TASK_HEADER
#define MPU_TASK_HEADER

#include <TaskManager.h>
#include <MPU6050Driver.h>
class CMPUTask:public CTask{
public:
  CMPUTask(   uint32_t                f_period
              ,CMPU6050Driver&         f_mpu6050)
  :CTask(f_period)
  ,m_mpu6050(f_mpu6050)
  {}

  void          _run(){
    m_data=m_mpu6050.readValue();
  }


  CMPU6050Driver::MPU6050_DATA get(){
    return m_data;
  }

private:
  CMPU6050Driver::MPU6050_DATA        m_data;
  CMPU6050Driver&                     m_mpu6050;
};

#endif
