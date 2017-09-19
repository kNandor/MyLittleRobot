#ifndef MOTOR_TASK_HEADER
#define MOTOR_TASK_HEADER

#include <MotorDriver.h>
#include <Controller.h>
#include <SpeedEncoderTask.h>

class CConvertorPWMVOLT{
  public:
    static float pwm2volt(float f_pwm){
      return pow(f_pwm,5)*(-1.386e-11)+pow(f_pwm,4)*(1.491e-08)+pow(f_pwm,3)*(-4.484e-06)+pow(f_pwm,2)*0.0002072+f_pwm*0.1056-4.28;
    }

    static uint8_t volt2pwm(float f_volt){
      float l_pwm=pow(f_volt,4)*(-0.001843)+pow(f_volt,3)*0.473+pow(f_volt,2)*(-4.503)+f_volt*24.36+27.24;
      if(l_pwm>255) return 255;
      else if(l_pwm<0) return 0;
      return static_cast<uint8_t>(l_pwm);
    }
};


class CPIDMotorController:public CTask{
public:
  CPIDMotorController   ( uint32_t                              f_period
                          ,CMotorDriver&                        f_motorDriver
                          ,CControllerFnc&                      f_controller
                          ,CSpeedEncoderFilterFnc&              f_encoderGetter)
                          :CTask(f_period)
                          ,m_ref_rps(0)
                          ,m_motorDriver(f_motorDriver)
                          ,m_controller(f_controller)
                          ,m_encoderGetter(f_encoderGetter)
                        {
                        }
  void       _run(){
    // l_rps - rotation per second
    float l_rps=m_encoderGetter.getFilteredRotation();

    float l_abs_ref=m_ref_rps>0?m_ref_rps:-m_ref_rps;
    float l_error=l_abs_ref-l_rps;
    float l_control_volt=m_controller.controlling(l_error);
    uint8_t l_pwm=0;
    bool isBraking=false;
    if(l_control_volt>12){
      l_pwm=255;
    }else if(l_control_volt>0){
      l_pwm=CConvertorPWMVOLT::volt2pwm(l_control_volt);
    }else{
      l_pwm=0;
      isBraking=true;
    }

    if(isBraking){
      m_motorDriver.setBraking(255);
    }else{
      if(m_ref_rps>0){
        m_motorDriver.setMotorPWMForward(l_pwm);
      }else{
        m_motorDriver.setMotorPWMBack(l_pwm);
      }
    }
    m_pwm=l_pwm;



  }

  void setRefRps        (float            f_ref_rps){
    m_ref_rps=f_ref_rps;
  }
  float getRefRps       (){
    return m_ref_rps;
  }
  uint8_t                                 m_pwm;
private:
  float                                 m_ref_rps;


  // double                                m_tt;
  CMotorDriver&                         m_motorDriver;
  CControllerFnc&                       m_controller;
  CSpeedEncoderFilterFnc&               m_encoderGetter;
};


// float l_rps=m_RigthEncoderTask.getData().rps;
// float l_rps_filt=m_FilteredEncoderTask.getFilteredRotation();
// float error=6.5-l_rps_filt;
// float l_control_volt=m_pidController.controlling(error);
// float l_control_pwm=0;
// if(l_control_volt>12){
//   l_control_volt=12;
//   l_control_pwm=CConvertorPWMVOLT::volt2pwm(l_control_volt);
//   if(l_control_pwm>255){
//     l_control_pwm=255;
//   }else if(l_control_pwm<0){
//     l_control_pwm=0;
//   }
//   m_motorDriverR.setMotorPWMForward(static_cast<uint8_t>(l_control_pwm));
// }else if(l_control_volt>0){
//   l_control_pwm=CConvertorPWMVOLT::volt2pwm(l_control_volt);
//   if(l_control_pwm>255){
//     l_control_pwm=255;
//   }else if(l_control_pwm<0){
//     l_control_pwm=0;
//   }
//   m_motorDriverR.setMotorPWMForward(static_cast<uint8_t>(l_control_pwm));
// }else{
//   m_motorDriverR.setBraking(255);
// }


#endif
