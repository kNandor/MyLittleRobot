
#ifndef FILTER_HEADER
#define FILTER_HEADER

#include <stdint.h>

#include "DiscreteSystem.h"

#ifndef PI
  #define PI 3.141593
#endif

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

    template<uint32_t N>
    class CButterworthFilter:public CDiscreteSystem<float,N,N>,public CFilterFnc{
      public:
        CButterworthFilter( float         f_w
                            ,float        f_dt)
        :CDiscreteSystem<float,N,N>()
        ,m_w(f_w)
        ,m_dt(f_dt)
        {
          MyMath::CPolynomialFunction<float,N> l_den=this->calculateDenominator();
          MyMath::CPolynomialFunction<float,N> l_num=this->calculateNumerator();
          float l_c=l_den[0];
          l_num=l_num/l_c;
          l_den=l_den/l_c;
          this->setCoefficients(l_num.getCoefficiences(),l_den.getCoefficiences());
        }

        float filtering (float f_input){
            return this->calculating(f_input);
        }

        float getValues(  uint32_t        f_filterOrder
                          ,uint32_t       f_indexValue
                          ,uint8_t        f_i){
            float l_angle=PI*(2*f_indexValue-1)/(2*f_filterOrder);
            switch(f_i){
              case 2:
                return  pow(m_dt,2)*pow(m_w,2)+4*m_dt*sin(l_angle)*m_w+4;
            }
        }

        MyMath::CPolynomialFunction<float,N> calculateDenominator(){
          if(N%2==0){
            uint32_t l_Nr=(N/2);
            Array<float,N+1> l_coeff;
            for(uint32_t i=0;i<N+1;++i){
              float l_coeff_p=polynomOrderCoeff(l_Nr,i,0);
              l_coeff[i]=l_coeff_p;
            }
            MyMath::CPolynomialFunction<float,N> l_polyFunc(l_coeff);
            return l_polyFunc;
          }
          else{
            uint32_t l_Nr=N/2;
            Array<float,N> l_coeff;
            for(uint32_t i=0;i<N;++i){
              float l_coeff_p=polynomOrderCoeff(l_Nr,i,0);
              l_coeff[i]=l_coeff_p;
            }
            MyMath::CPolynomialFunction<float,N-1> l_polyFunc(l_coeff);
            Array<float,2> l_c1={m_w*m_dt+2,m_w*m_dt-2};
            MyMath::CPolynomialFunction<float,1> l_p1(l_c1);
            MyMath::CPolynomialFunction<float,N> l_polRes=l_polyFunc*l_p1;
            return l_polRes;
          }
        }


        float polynomOrderCoeff(uint32_t m,uint32_t n,uint32_t l){
          if(n==0){
            float res=1;
            for(uint32_t i=l;i<m;++i){
              res*=polCoeff(i,0);
            }
            return res;
          }else if(n==1){
            if(l==m-1){
              return polCoeff(l,1);
            }else{
              return polCoeff(l,0)*polynomOrderCoeff(m,n,l+1)+polCoeff(l,1)*polynomOrderCoeff(m,n-1,l+1);
            }
          }else if(n==2){
            if(l==m-1){
              return polCoeff(l,2);
            }else{
              return polCoeff(l,0)*polynomOrderCoeff(m,n,l+1)+polCoeff(l,1)*polynomOrderCoeff(m,n-1,l+1)+polCoeff(l,2)*polynomOrderCoeff(m,n-2,l+1);
            }
          }else{
            if(n%2==1){
              if(l>=m-((n+1)/2)){
                return polCoeff(l,1)*polynomOrderCoeff(m,n-1,l+1)+polCoeff(l,2)*polynomOrderCoeff(m,n-2,l+1);
              }else{
                return polCoeff(l,0)*polynomOrderCoeff(m,n,l+1)+polCoeff(l,1)*polynomOrderCoeff(m,n-1,l+1)+polCoeff(l,2)*polynomOrderCoeff(m,n-2,l+1);
              }
            }else{
              if(l>=m-((n+1)/2)){
                return polCoeff(l,2)*polynomOrderCoeff(m,n-2,l+1);
              }else{
                return polCoeff(l,0)*polynomOrderCoeff(m,n,l+1)+polCoeff(l,1)*polynomOrderCoeff(m,n-1,l+1)+polCoeff(l,2)*polynomOrderCoeff(m,n-2,l+1);
              }
            }
          }
        }

        float polCoeff(uint32_t k,uint8_t i){
          float angle=PI*(2.0*(k+1)-1.0)/(2.0*N);
          switch(i){
            case 0:
              return pow(m_dt,2)*pow(m_w,2)+4*m_dt*sin(angle)*m_w+4;
            case 1:
              return 2*pow(m_dt,2)*pow(m_w,2)-8;
            default:
              return 4-4*m_dt*sin(angle)*m_w+pow(m_w,2)*pow(m_dt,2);

          }
        }


        MyMath::CPolynomialFunction<float,N> calculateNumerator(){
          uint32_t l_Nr=static_cast<uint32_t>(N/2);
          float l_realCoef=pow(m_dt,2*l_Nr)*pow(m_w,N);
          if(N%2==0){
            Array<float,N+1> l_coeff=MyMath::binomialCoefficient<N>();
            MyMath::CPolynomialFunction<float,N> l_polRes(l_coeff);
            l_polRes=l_polRes*l_realCoef;
            return l_polRes;
          }else{
            MyMath::CPolynomialFunction<float,N-1> l_p(MyMath::binomialCoefficient<N-1>());
            l_p=l_p*l_realCoef;
            Array<float,2> l_coeff2={m_dt,m_dt};
            MyMath::CPolynomialFunction<float,1> l_p2(l_coeff2);
            MyMath::CPolynomialFunction<float,N> l_polRes=l_p*l_p2;
            return l_polRes;
          }
        }
      float m_w;
      float m_dt;
    };


    // class CFilterLowPass:public CDiscreteSystem<float,3,3> , public CFilterFnc{
    // public:
    //   CFilterLowPass( float         f_omega
    //                   ,float        f_q
    //                   ,float        f_dt)
    //                   :CDiscreteSystem<float,3,3>()
    //   {
    //     float l_temp=(4.f/(f_dt*f_dt)+(2.f*f_omega)/(f_q*f_dt)+f_omega*f_omega);
    //     float l_a0=f_omega*f_omega/l_temp;
    //     float l_b1=(2.f*f_omega*f_omega-8.f/(f_dt*f_dt))/l_temp;
    //     float l_b2=((4.f/(f_dt*f_dt)+(f_omega*f_omega)-(2.f*f_omega)/(f_dt*f_q)))/l_temp;

    //     Array<float,3> l_num={l_a0,2.f*l_a0,l_a0};
    //     Array<float,3> l_denum={1.f,l_b1,l_b2};
    //     this->setCoefficients(l_num,l_denum);
    //   }

    //   float filtering (float f_input){
    //     return calculating(f_input);
    //   }
    // };
  };
};


#endif
