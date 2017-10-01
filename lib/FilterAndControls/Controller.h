#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include "../Util/PolynomialFunction.h"
#include "DiscreteSystem.h"



// float VOLT2PWM(float f_volt){
//   return pow(f_volt,4)*(-0.001843)+pow(f_volt,3)*0.473+pow(f_volt,2)*(-4.503)+f_volt*24.36+27.24;
// }


class CControllerFnc{
public:
  virtual float controlling(float f_error)=0;
};

class CPIDController:public CControllerFnc{
public:
  CPIDController(float          f_kp
                  ,float        f_ki
                  ,float        f_kd
                  ,float        f_tf
                  ,float        f_dt)
                  :m_kp(f_kp)
                  ,m_Isys({0,f_ki*f_dt},{1,-1})
                  ,m_Dsys({f_kd,-1*f_kd},{f_tf+f_dt,-1*f_tf})
  {
    // Array<float,2> l_numI,l_denI,l_numD,l_denD;
    // l_numI={f_ki*f_dt,0};l_denI={1,-1};
    // l_numD={f_kd*f_N,-f_kd*f_N};l_denD={1,-1+f_N*f_dt};
    // m_Isys.setCoefficients({f_ki*f_dt,0},{1,-1});
    // m_Dsys.setCoefficients({f_kd*f_N,-f_kd*f_N},{1,-1+f_N*f_dt});
  }

  float controlling(float f_error){
    float l_kp_res=f_error*m_kp;
    m_error=f_error;
    float l_ki_res=m_Isys.calculating(f_error);
    float l_kd_res=m_Dsys.calculating(f_error);
    return l_kp_res+l_ki_res+l_kd_res;
  }
  float getError(){
    return m_error;
  }

private:
  const float                       m_kp;
  float                             m_error;
  CDiscreteSystem<float,1,1>        m_Isys;
  CDiscreteSystem<float,1,1>        m_Dsys;
};





// template<class T>
// class CPIDController:public CDiscreteSystem<T,3,3>, public CControllerFnc{
//   public:
//     CPIDController(   float           f_kp
//                       ,float          f_ki
//                       ,float          f_kd
//                       ,float          f_df
//                       ,float          f_dt)
//                       :CDiscreteSystem<T,3,3>()
//   {
//     float a0=(1+f_df*f_dt/2)*(f_kp+f_ki*f_dt/2);
// 		float a1=-2*f_kp+f_ki*f_dt/2*f_df*f_dt+f_df;
//     float	a2=(1-f_df*f_dt/2)*(f_kp-f_ki*f_dt/2)-f_df;
// 		float b0=(1+f_df*f_dt/2);
// 		float b1=-2;
// 		float b2=(1-f_df*f_dt/2);
//     // Array<float,3> l_poliNum={f_kp*f_df+f_kd,f_kp*(f_dt-2*f_df)+f_ki*f_dt*f_df-2*f_kd,f_kp*(f_df-f_dt)+(f_dt-f_df)*f_ki*f_dt+f_kd};
//     // Array<float,3> l_poliDenum={f_df,(f_dt-2*f_df),(f_df-f_dt)};
//     // Array<float,3> l_poliNum={-f_df*f_dt*f_ki+f_df*f_kp+f_dt*f_dt*f_ki-f_dt*f_kp+f_kd
//     //                           ,f_dt*f_dt*f_ki-2*f_df*f_kp+f_dt*f_kp-2*f_kd
//     //                           ,f_df*f_kp+f_kd};
//     // Array<float,3> l_poliDenum={f_df-f_dt
//     //                             ,-2*f_df+f_dt
//     //                             ,f_df};
//      Array<float,3> l_poliNum={a0,a1,a2};
//      Array<float,3> l_poliDenum={b0,b1,b2};
//     this->setCoefficients(l_poliNum,l_poliDenum);
//   }

//   float controlling(float f_error){
//     return this->calculating(f_error);
//   }
// };

// template<class T>
// class CPIController:public CDiscreteSystem<T,2,2>, public CControllerFnc{
// public:
//   CPIController(   float        f_kp
//                     ,float      f_ki
//                     ,float      f_dt)
//                     :CDiscreteSystem<T,2,2>()
//   {
//     Array<float,2> l_poliNum={2*f_kp+f_ki*f_dt,(f_ki*f_dt-2*f_kp)};
//     Array<float,2> l_poliDenum={2,-2};
//     this->setCoefficients(l_poliNum,l_poliDenum);
//   }
//   float controlling(float f_error){
//     return this->calculating(f_error);
//   }
// };

#endif
