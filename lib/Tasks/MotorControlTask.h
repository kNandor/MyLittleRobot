#ifndef MOTOR_CONTROL_TASK_HEADER
#define MOTOR_CONTROL_TASK_HEADER

#include <MotorDriver.h>
#include <PolynomialFunction.h>

class CConvertorPWMVOLT{
  public:
    static float pwm2volt(float pwm){
      float volt=6.187e-09*pow(pwm,4)-2.556e-06*pow(pwm,3)+2.847e-05*pow(pwm,2)+0.1132*pwm-4.386;
      return volt<0?0:volt;
    }

    static float volt2pwm(float volt){
      return -0.007944*pow(volt,4)+0.587*pow(volt,3)-5.433*pow(volt,2)+27.3*volt+24.37;
    }
};


class CPIDMotorController:public CTask{
public:
  CPIDMotorController   ( uint32_t                  f_period
                          ,CMotorDriver&            f_motorDriver
                          ,CControllerFnc&          f_pidController
                          ,CSpeedGetterFnc&         f_rpsGetter)
                          :CTask(f_period)
                          ,m_motorDriver(f_motorDriver)
                          ,m_pidController(f_pidController)
                          ,m_rpsGetter(f_rpsGetter)
                        {
                          m_rotation=0.0;
                          m_pwm=0.0;
                        }
  void       _run(){
      float l_rps=m_rpsGetter.getData().rps;
      bool  l_dir=m_rotation>=0;
      float l_rot=abs(m_rotation);
      float l_error=l_rot-l_rps;
      float l_voltage=m_pidController.controlling(l_error);
      float l_pwm=CConvertorPWMVOLT::volt2pwm(l_voltage);
      if(l_pwm>255){
        l_pwm=255;
      }else if(l_pwm<0){
        l_pwm=0;
      }
      m_pwm=l_pwm;
      if(l_dir){
        m_motorDriver.setMotorPWMForward(l_pwm);
      }else{
        m_motorDriver.setMotorPWMBack(l_pwm);
      }
  }

  float getRotation(){return m_rotation;}
  void  setRotation(float f_rotation){
    m_rotation=f_rotation;
  }
  float getPWM(){return m_pwm;}
  float                                        m_rotation;
private:
  CMotorDriver&                                m_motorDriver;
  CControllerFnc&                              m_pidController;
  CSpeedGetterFnc&                             m_rpsGetter;
  float                                        m_pwm;
};

class CPIMotorControllerWithFilter:public CTask{
public:
  CPIMotorControllerWithFilter( uint32_t                    f_period
                                ,CMotorDriver&              f_motorDriver
                                ,CControllerFnc&            f_controlFnc
                                ,CSpeedEncoderFilterFnc&    f_getFilterDataFnc)
                                :CTask(f_period)
                                ,m_motorDriver(f_motorDriver)
                                ,m_controlFnc(f_controlFnc)
                                ,m_getFilterDataFnc(f_getFilterDataFnc)
  {
    m_rotation=0;
    m_pwm=0;
  }

  void      _run(){
    float l_rotation=m_getFilterDataFnc.getFilteredRotation();
    float l_error=abs(m_rotation)-l_rotation;
    float l_signal=m_controlFnc.controlling(l_error);
    if(l_signal>255){
      l_signal=255;
    }else if(l_signal<0){
      l_signal=0;
    }
    uint8_t l_pwm=l_signal;
    m_pwm=l_pwm;
    if(m_rotation>0){
      m_motorDriver.setMotorPWMForward(l_pwm);
    }
    else{
      m_motorDriver.setMotorPWMBack(l_pwm);
    }
  }
  float   getRotation(){
    return m_rotation;
  }

  void    setRotation(float f_rotation){
    m_rotation=f_rotation;
  }

  uint8_t getPWM(){
    return m_pwm;
  }
private:
  CMotorDriver&               m_motorDriver;
  CControllerFnc&             m_controlFnc;
  CSpeedEncoderFilterFnc&     m_getFilterDataFnc;
  float                       m_rotation;
  uint8_t                     m_pwm;
};

#endif
