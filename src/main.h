#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <Arduino.h>
#include <Util.h>
#include <IncrementalEncoder.h>
#include <EncoderTask.h>
#include <TaskManager.h>
#include <MotorDriver.h>
#include <UtilTasks.h>


#define NR_TASKS 3


class _Main_{
public:
  CInterruptCounter                         m_incrementalLeft;


private:

//---DRIVERS------------------------------------------------------------------------------
  //-----------------MOTOR DRIVERS--------------------------------------------------------
  CMotorDriver                              m_motorDriverR;
  CMotorDriver                              m_motorDriverL;
  //--------------------------------------------------------------------------------------

  //----------------PWM to Voltage converter----------------------------------------------


  //--------------------------------------------------------------------------------------

  //----------------TASKS AND TASK MANAGERS-------------------------------------------------
  //Base tick definition
  const float                               m_baseTick;
  //-----------------STANDARD  CLASSES----------------------------------------------------
  //Periodic led blinker Class
    CBlinker                                  m_builtinLedBlinker;
  //Serial Handler Class
    CSerialHandler                            m_serialHandler;
  //--------------------------------------------------------------------------------------
  //----------------SENSOR TASK ----------------------------------------------------------
    CEncoderTask                            m_leftEncoderTask;
  //----------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------

  //TASK MANAGER
  CTask                                     *m_tasks[NR_TASKS];//,&g_speedFilterL,&g_motorControllerL,&g_speedFilterR,&g_motorControllerR};
  CTaskManager                              m_taskManager;
  //
//----------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
//Temporization
  uint32_t m_timestamp_us;
  const uint32_t m_interval_us;
  //Rectagular
  uint32_t m_timestamp_step_us;
  uint8_t  m_PWM;
  uint8_t  m_MaxPWM=250;
//---------------------------------------------------------------------------------------

public:
  _Main_();
  void loop();
  void setup();
private:
  void readSensors();
  void stepChanger();
  void periodicSender();
};

_Main_::_Main_()
  :m_incrementalLeft(2,incrementalLeftSensor)
// Drivers
  ,m_motorDriverR(6,8,12)
  ,m_motorDriverL(5,4,7)
// Tasks
  ,m_baseTick(8e-5)
  ,m_builtinLedBlinker(LED_BUILTIN,1.0/m_baseTick)
  ,m_serialHandler(4e-4/m_baseTick,Serial)
  ,m_leftEncoderTask(0.01/m_baseTick,0.01,40,m_incrementalLeft)
  ,m_tasks({&m_builtinLedBlinker,&m_serialHandler,&m_leftEncoderTask})
  ,m_taskManager(m_baseTick,NR_TASKS,m_tasks)
  ,m_interval_us(0.1e6)
{
  // m_interval_us=0.1e6;
  m_PWM=0;
}

//15.5 encoder and real wheels rotation
void _Main_::setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(921600);
  m_timestamp_us=micros();
  m_timestamp_step_us=micros();
  return;
}

void _Main_::loop(){
  stepChanger();
  periodicSender();
  m_taskManager.run();
  return;
}


void _Main_::stepChanger(){
  uint32_t l_timestamp_us=micros();
  if(l_timestamp_us-m_timestamp_step_us>6.e6){
    m_timestamp_step_us=l_timestamp_us;
    if(m_PWM==0){
      m_PWM=m_MaxPWM;
      // if(m_MaxPWM<250) m_MaxPWM+=10;
      // else{
        // m_MaxPWM=0;
      // }
    }else{
      // m_PWM=0;
    }
  }
}

void _Main_::periodicSender(){
  uint32_t l_timestamp_us=micros();
  if(l_timestamp_us-m_timestamp_us>m_interval_us){
    m_timestamp_us=l_timestamp_us;
    float l_rps=m_leftEncoderTask.getRps();
    m_serialHandler.append("%s\n",String(l_rps,2).c_str());
  }
}

_Main_                                    mainObject;
void incrementalLeftSensor(){
  mainObject.m_incrementalLeft.increment();
}


#endif
