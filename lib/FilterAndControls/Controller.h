#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include <PolynomialFunction.h>
#include <DiscreteSystem.h>

class CControllerFnc{
public:
  virtual float controlling(float f_error)=0;
};


template<class T>
class CPIDController:public CDiscreteSystem<T,3,3>, public CControllerFnc{
  public:
    CPIDController(   float           f_kp
                      ,float          f_ki
                      ,float          f_kd
                      ,float          f_df
                      ,float          f_dt)
                      :CDiscreteSystem<T,3,3>()
  {
    float a0=(1+f_df*f_dt/2)*(f_kp+f_ki*f_dt/2);
		float a1=-2*f_kp+f_ki*f_dt/2*f_df*f_dt+f_df;
    float	a2=(1-f_df*f_dt/2)*(f_kp-f_ki*f_dt/2)-f_df;
		float b0=(1+f_df*f_dt/2);
		float b1=-2;
		float b2=(1-f_df*f_dt/2);
    // Array<float,3> l_poliNum={f_kp*f_df+f_kd,f_kp*(f_dt-2*f_df)+f_ki*f_dt*f_df-2*f_kd,f_kp*(f_df-f_dt)+(f_dt-f_df)*f_ki*f_dt+f_kd};
    // Array<float,3> l_poliDenum={f_df,(f_dt-2*f_df),(f_df-f_dt)};
    // Array<float,3> l_poliNum={-f_df*f_dt*f_ki+f_df*f_kp+f_dt*f_dt*f_ki-f_dt*f_kp+f_kd
    //                           ,f_dt*f_dt*f_ki-2*f_df*f_kp+f_dt*f_kp-2*f_kd
    //                           ,f_df*f_kp+f_kd};
    // Array<float,3> l_poliDenum={f_df-f_dt
    //                             ,-2*f_df+f_dt
    //                             ,f_df};
     Array<float,3> l_poliNum={a0,a1,a2};
     Array<float,3> l_poliDenum={b0,b1,b2};
    this->setCoefficients(l_poliNum,l_poliDenum);
  }

  float controlling(float f_error){
    return this->calculating(f_error);
  }
};

template<class T>
class CPIController:public CDiscreteSystem<T,2,2>, public CControllerFnc{
public:
  CPIController(   float        f_kp
                    ,float      f_ki
                    ,float      f_dt)
                    :CDiscreteSystem<T,2,2>()
  {
    Array<float,2> l_poliNum={2*f_kp+f_ki*f_dt,(f_ki*f_dt-2*f_kp)};
    Array<float,2> l_poliDenum={2,-2};
    this->setCoefficients(l_poliNum,l_poliDenum);
  }
  float controlling(float f_error){
    return this->calculating(f_error);
  }
};

#endif
