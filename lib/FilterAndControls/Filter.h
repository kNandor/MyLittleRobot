
#ifndef FILTER_HEADER
#define FILTER_HEADER

#include <stdint.h>

#include <DiscreteSystem.h>

namespace Filter{
  namespace FIR{
    //LOW-PASS FILTER
    // wc   -   cutt-off freqvency [rad/s]
    //          - wc=2*pi*F0, where F0 is the cutt-off freqvency
    // q    -   quality factor of filter
    //          -zeta=2/q -dumping ratio
    //          -q>0.707 ->peaking
    //          -q<0.707 ->gentle slope
    class CFilterFnc{
    public:
      virtual float filtering(float)=0;
    };
    class CFilterLowPass:public CDiscreteSystem<float,3,3> , public CFilterFnc{
    public:
      CFilterLowPass( float         f_omega
                      ,float        f_q
                      ,float        f_dt)
                      :CDiscreteSystem<float,3,3>()
      {
        float l_temp=(4.f/(f_dt*f_dt)+(2.f*f_omega)/(f_q*f_dt)+f_omega*f_omega);
        float l_a0=f_omega*f_omega/l_temp;
        float l_b1=(2.f*f_omega*f_omega-8.f/(f_dt*f_dt))/l_temp;
        float l_b2=((4.f/(f_dt*f_dt)+(f_omega*f_omega)-(2.f*f_omega)/(f_dt*f_q)))/l_temp;

        Array<float,3> l_num={l_a0,2.f*l_a0,l_a0};
        Array<float,3> l_denum={1.f,l_b1,l_b2};
        this->setCoefficients(l_num,l_denum);
      }

      float filtering (float f_input){
        return calculating(f_input);
      }
    };
  };
};


#endif
