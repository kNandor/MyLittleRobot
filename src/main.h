#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <Arduino.h>
#include <Util.h>
#include <SpeedEncoder.h>
#include <TaskManager.h>
#include <MotorDriver.h>
#include <SpeedEncoderTask.h>
#include <UtilTasks.h>
#include <Filter.h>
#include <Controller.h>
#include <MotorControlTask.h>

#define NR_TASKS 2



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
  // MyMath::CPolynomialFunction<float,3>      m_pwm2Volt;

  //--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

//----------------TASKS AND TASK MANAGERS-------------------------------------------------
  //Base tick definition
  const float                               m_baseTick;
  //-----------------STANDARD  CLASSES----------------------------------------------------
  //Periodic led blinker Class
    CBlinker                                  m_builtinLedBlinker;
  //Serial Handler Class
    CSerialHandler                            m_serialHandler;
  //--------------------------------------------------------------------------------------
  //-----------------SENSORS DRIVERS------------------------------------------------------
    // CSpeedEncoderReadTask                     m_speedReaderL;
    // CSpeedEncoderReadTask                     m_speedReaderR;
  //--------------------------------------------------------------------------------------
  //----------------CPID-COntrollers------------------------------------------------------
  //  CPIDController<float>                      m_pidControllerL;
  //  CPIDMotorController                        m_motorControllerL;
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
  uint16_t m_voltageRead=0;
  uint32_t m_timestamp_step_us;
  uint8_t  m_PWM;
  uint8_t  m_MaxPWM=250;
//---------------------------------------------------------------------------------------

public:
  _Main_();
  void loop();
  void setup();

  void run();
//Always runned
  void readSensors();
  void stepChanger();
};

_Main_::_Main_()
  :m_incrementalLeft(2,increment1)
// Drivers
  ,m_motorDriverL(5,4,7)
  ,m_motorDriverR(6,8,12)
// Tasks
  ,m_baseTick(8e-5)
  ,m_builtinLedBlinker(LED_BUILTIN,1.0/m_baseTick)
  ,m_serialHandler(4e-4/m_baseTick,Serial)
  // Readers
  // ,m_speedReaderL(0.025/m_baseTick,0.025,10,40)
  // ,m_speedReaderR(0.025/m_baseTick,0.025,9,40)
  // PID-Controller
  // ,m_pidControllerL(0.0617,0.134,0.00056,0.001,0.005)
  // ,m_motorControllerL(0.005/m_baseTick,m_motorDriverL,m_pidControllerL,m_speedReaderL)
  ,m_tasks({&m_builtinLedBlinker,&m_serialHandler})
  ,m_taskManager(m_baseTick,NR_TASKS,m_tasks)
  ,m_interval_us(0.1e6)
{

  // m_tasks=new CTask[2];
  // m_tasks={&m_builtinLedBlinker,&m_serialHandler};
  m_PWM=0;
  m_motorControllerL.m_rotation=100.0;
  m_motorControllerL.setRotation(100.0);
}
//15.5 encoder and real wheels rotation
void _Main_::setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(2,OUTPUT);pinMode(3,OUTPUT);pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);pinMode(7,OUTPUT);pinMode(8,OUTPUT);
  pinMode(9,INPUT);pinMode(10,INPUT);

  Serial.begin(921600);
  m_timestamp_us=micros();
  m_timestamp_step_us=micros();
  return;
}

void _Main_::loop(){
  readSensors();
  stepChanger();
  uint32_t l_timestamp_us=micros();
  if(l_timestamp_us-m_timestamp_us>m_interval_us){
    // CSpeedData l_data=m_speedReaderL.getData();
    // float l_pwm=m_motorControllerL.getPWM();
    // float l_voltage=m_voltageRead/1023.0*5.0;
    // m_serialHandler.append("%ld %s %s\n",l_data.timestamp_us,String(l_data.rps,2).c_str(),String(l_pwm,0).c_str());
    // m_timestamp_us=l_timestamp_us;
  }
  m_taskManager.run();
  return;
}

void _Main_::run(){
  m_taskManager.run();
}
void _Main_::readSensors(){
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


#endif
