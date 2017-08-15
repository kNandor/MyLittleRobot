#ifndef SPEED_ENCODER_HEADER
#define SPEED_ENCODER_HEADER

typedef struct{
  uint32_t    timestamp_us;
  float       rps;
}CSpeedData;

class CSpeedGetterFnc{
public:
  virtual CSpeedData getData()=0;
};

class CNumarator{
  public:
    CNumarator( uint8_t     f_pin)
                    :m_pin(f_pin)
    {
      m_state=(digitalRead(m_pin)==HIGH);
      m_nrSemnal=0;
    }

    void readSensor(){
      bool l_state=(digitalRead(m_pin)==HIGH);
      if(l_state!=m_state){
          m_state=l_state;
          m_nrSemnal++;
      }
    }

    uint32_t getNrSemnal(){
      return m_nrSemnal;
    }

    void reset(){m_nrSemnal=0;}
  private:
    const uint8_t   m_pin;
    bool            m_state;
    uint32_t        m_nrSemnal;
};

class CSpeedEncoder:public CSpeedGetterFnc{
public:
  CSpeedEncoder(  uint8_t         f_pin
                  ,uint8_t        f_nrSPR)
                  :m_pin(f_pin)
                  ,m_nrSPR(f_nrSPR)
  {
    m_state=(digitalRead(m_pin)==HIGH);
    m_timestamp_us=micros();
    m_duration=0;
    m_newAvaible=false;
    m_speedData.timestamp_us=0;
    m_speedData.rps=0;
    m_nrSemnal=0;
  }

  void readSensor(){
    bool l_state=(digitalRead(m_pin)==HIGH);
    if(l_state!=m_state){
        uint32_t l_timestamp_us=micros();
        m_duration=l_timestamp_us-m_timestamp_us;
        m_timestamp_us=l_timestamp_us;
        m_speedData.timestamp_us=m_timestamp_us;
        float l_rotation=1.0/(static_cast<float>(m_duration*m_nrSPR)*1.e-6);
        if(l_rotation/15.5*60.0<520){
            m_speedData.rps=1.0/(static_cast<float>(m_duration*m_nrSPR)*1.e-6);
        }
        m_newAvaible=true;
        m_state=l_state;
        m_nrSemnal++;
    }else{
      uint32_t l_timestamp_us=micros();
      uint32_t l_duration=l_timestamp_us-m_timestamp_us;
      if(l_duration>m_duration*10){
        m_duration=l_duration;
        m_speedData.rps=1.0/(static_cast<float>(m_duration*m_nrSPR)*1.e-6);
        m_newAvaible=true;
      }
    }
  }

  bool isNewAvaible(){return m_newAvaible;}
  uint32_t getDuration(){
    m_newAvaible=false;
    return m_duration;
  }

  virtual CSpeedData getData(){
    m_newAvaible=false;
    return m_speedData;
  }

  uint32_t getNrSemnal(){
     return m_nrSemnal;
  }

  void resetNumarator(){
    m_nrSemnal=0;
  }

private:
  const uint8_t       m_pin;
  const uint8_t       m_nrSPR;
  bool                m_state;
  bool                m_newAvaible;
  uint32_t            m_timestamp_us;
  uint32_t            m_duration;

  uint32_t            m_nrSemnal;
  CSpeedData          m_speedData;

};

#endif
