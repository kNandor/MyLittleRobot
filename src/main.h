#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Util.h>
#include <EncoderTask.h>
#include <TaskManager.h>
#include <MotorDriver.h>
#include <UtilTasks.h>
// #include <Filter.h>
#include <SpeedEncoderTask.h>
#include <Controller.h>
#include <MotorTask.h>
#include <MPUTask.h>


#define NR_TASKS 10
 // #define NR_TASKS 3


class _Main_{
public:
  CInterruptCounter                         m_incrementalLeft;
  CInterruptCounter                         m_incrementalRight;

private:
  SoftwareSerial                            m_bluetooth;

//---DRIVERS------------------------------------------------------------------------------
  //-----------------MOTOR DRIVERS--------------------------------------------------------
  CMotorDriver                              m_motorDriverR;
  CMotorDriver                              m_motorDriverL;
  //--------------------------------------------------------------------------------------

  //-----------------Accel and Gry DRIVERS------------------------------------------------
  CMPU6050Driver                            m_mpuDriver;
  //--------------------------------------------------------------------------------------

  //-----------------FILTER BUTTERWORTH---------------------------------------------------
  Filter::FIR::CButterworthFilter<1>        m_rightFilter;
  Filter::FIR::CButterworthFilter<1>        m_leftFilter;
  //--------------------------------------------------------------------------------------
  //-----------------PID CONTROLLER ------------------------------------------------------
  CPIDController                            m_pidRight;
  CPIDController                            m_pidLeft;
  //--------------------------------------------------------------------------------------

  //----------------TASKS AND TASK MANAGERS-----------------------------------------------
    //Base tick definition
    const float                               m_baseTick;
  //-----------------STANDARD  CLASSES----------------------------------------------------
    //Periodic led blinker Class
    CBlinker                                  m_builtinLedBlinker;
    CRunTester                                m_runTest;
    //Serial Handler Class
    CSerialHandler                            m_serialHandler;
  //--------------------------------------------------------------------------------------
  //----------------MPU Task ----------------------------------------------------------
    CMPUTask                                  m_mpuTask;
  //--------------------------------------------------------------------------------------
  //----------------SENSOR TASK ----------------------------------------------------------
    CEncoderTask                              m_RigthEncoderTask;
    CEncoderTask                              m_LeftEncoderTask;
  //--------------------------------------------------------------------------------------
  //----------------Filtered Encoder -----------------------------------------------------
    CSpeedEncoderFilterTask                   m_FilterRightTask;
    CSpeedEncoderFilterTask                   m_FilterLeftTask;
  //--------------------------------------------------------------------------------------
    CPIDMotorController                       m_mctlRight;
    CPIDMotorController                       m_mctlLeft;
  //--------------------------------------------------------------------------------------

  //----------------TASK MANAGER----------------------------------------------------------
  CTask                                      *m_tasks[NR_TASKS];
  CTaskManager                                m_taskManager;
  //--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
//Temporization
  uint32_t m_timestamp_us;
  const uint32_t m_interval_us;
  //Rectagular
  uint32_t m_timestamp_step_us;
  uint8_t  m_PWM;
  uint8_t  m_MaxPWM=250;
  uint32_t m_Num=0;
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
  :m_bluetooth(0,1)
  ,m_incrementalLeft(2,incrementalLeftSensor)
  ,m_incrementalRight(3,incrementalRightSensor)
// Drivers
  ,m_motorDriverR(6,8,12)
  ,m_motorDriverL(5,4,7)
//Encoder Filters
  ,m_rightFilter(10.5,0.0075)
  ,m_leftFilter(10.5,0.0075)
//Pid controller
  ,m_pidRight(7.669549199439247, 28.489911535208886, 0.33785752719167905,150,0.0075)
  ,m_pidLeft(7.669549199439247, 28.489911535208886, 0.33785752719167905,150,0.0075)
// Tasks
  ,m_baseTick(5.0e-5)
  ,m_runTest(0.0070/m_baseTick,0.0070)
  ,m_builtinLedBlinker(LED_BUILTIN,1.0/m_baseTick)
  ,m_serialHandler(2.0e-4/m_baseTick,Serial)
  ,m_mpuTask(0.0075/m_baseTick,m_mpuDriver)
  ,m_RigthEncoderTask(0.0075/m_baseTick,0.0075,40,m_incrementalRight)
  ,m_LeftEncoderTask(0.0075/m_baseTick,0.0075,40,m_incrementalLeft)
  ,m_FilterRightTask(0.0075/m_baseTick,m_RigthEncoderTask,m_rightFilter)
  ,m_FilterLeftTask(0.0075/m_baseTick,m_LeftEncoderTask,m_leftFilter)
  ,m_mctlRight(0.0075/m_baseTick,m_motorDriverR,m_pidRight,m_FilterRightTask)
  ,m_mctlLeft(0.0075/m_baseTick,m_motorDriverL,m_pidLeft,m_FilterLeftTask)
  ,m_tasks({&m_builtinLedBlinker,&m_runTest,&m_serialHandler,&m_mpuTask,&m_RigthEncoderTask,&m_FilterRightTask,&m_mctlRight,&m_LeftEncoderTask,&m_FilterLeftTask,&m_mctlLeft})
  //
  ,m_taskManager(m_baseTick,NR_TASKS,m_tasks)
  ,m_interval_us(0.05e6)
{
  // m_interval_us=0.1e6;
  m_PWM=0;
}

//15.5 encoder and real wheels rotation
void _Main_::setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(921600);

  pinMode(0,INPUT);
  pinMode(1,OUTPUT);
  // m_bluetooth.begin(9600);

  m_timestamp_us=micros();
  m_timestamp_step_us=micros();
  Wire.begin();
  m_mpuDriver.init();
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
  if(l_timestamp_us-m_timestamp_step_us>2.e6){
    m_mctlRight.setRefRps(0.0f);
    m_mctlLeft.setRefRps(0.0f);

  }
  else{
    m_mctlRight.setRefRps(4.0f);
    m_mctlLeft.setRefRps(4.0f);
  }
}

void _Main_::periodicSender(){
  // m_mctlRight.setRefRps(2.0f);
  // m_mctlLeft.setRefRps(2.0f);
  // m_motorDriverL.setMotorPWMForward(55);
  // m_mctlRight.getRefRps();
  // m_motorDriverR.setMotorPWMForward(255);
  uint32_t l_timestamp_us=micros();
  if(l_timestamp_us-m_timestamp_us>m_interval_us){
      // uint32_t l_dur=m_runTest.getDuration();
      float l_rpsRight=m_FilterRightTask.getFilteredRotation();
      float l_rpsLeft=m_FilterLeftTask.getFilteredRotation();
      CMPU6050Driver::MPU6050_DATA l_data=m_mpuTask.get();
      m_serialHandler.append("%s,%s,%s,%s,%s,%s,%s,%s\n"
                                          ,String(l_rpsRight,2).c_str()
                                          ,String(l_rpsLeft,2).c_str()
                                          ,String(l_data.x_accel,2).c_str()
                                          ,String(l_data.y_accel,2).c_str()
                                          ,String(l_data.z_accel,2).c_str()
                                          ,String(l_data.x_gyro,2).c_str()
                                          ,String(l_data.y_gyro,2).c_str()
                                          ,String(l_data.z_gyro,2).c_str()
                                        );
      // m_serialHandler.append("HHHHH HHHHH \n");

      // if(m_runTest.get()>0){
      //   m_serialHandler.append("e%ld\n",m_runTest.get());
      //   m_runTest.reset();
      // }
      // m_serialHandler.append("%s,%s\n",String(l_rpsRight,2).c_str(),String(l_rpsLeft,2).c_str());
      // m_serialHandler.append("%s,%s,%s\n",String(error,2).c_str(),String(l_control_volt,2).c_str(),String(l_rps_filt,1).c_str());
      // if(m_PWM==0){
      //   float volt=-100.0/255.0;
      //
      // }else{
      //   float volt=100.0/255.0;
      //   m_serialHandler.append("%s,%s\n",String(volt,2).c_str(),String(l_rps_filt,1).c_str());
    // }

    // }
    m_timestamp_us=l_timestamp_us;
  }
}

_Main_                                    mainObject;
void incrementalLeftSensor(){
  mainObject.m_incrementalLeft.increment();
}

void incrementalRightSensor(){
  mainObject.m_incrementalRight.increment();
}

#endif
