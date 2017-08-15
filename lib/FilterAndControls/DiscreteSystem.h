#ifndef DISCRETE_SYSTEM_HEADER
#define DISCRETE_SYSTEM_HEADER

#include <PolynomialFunction.h>

template<class T,uint32_t N>
class CProcessMemories{
public:
  CProcessMemories()
  {
    setZeros();
  }

  const T operator[](const uint32_t f_index){
    return m_values[f_index];
  }

  T operator[](const uint32_t f_index) const{
    return m_values[f_index];
  }

  void operator<<(T f_value){
      for(int i=N-1;i>0;--i){
        m_values[i]=m_values[i-1];
      }
      m_values[0]=f_value;
  }
private:
  void setZeros(){
    for(uint32_t i=0;i<N;++i){
      m_values[i]=static_cast<T>(0);
    }
  }


  T m_values[N];
};

template<class T,uint32_t N,uint32_t M>
class CDiscreteSystem{
public:
  CDiscreteSystem(  Array<T,N> f_num
                    ,Array<T,M> f_denum
                    )
                    :m_input()
                    ,m_output()
                    ,m_num(f_num)
                    ,m_denum(f_denum)
  {
  }

  CDiscreteSystem(  MyMath::CPolynomialFunction<T,N-1>    f_num
                    ,MyMath::CPolynomialFunction<T,M-1>   f_denum)
                    :m_input()
                    ,m_output()
                    ,m_num(f_num)
                    ,m_denum(f_denum)
  {}



  CDiscreteSystem()
                  :m_input()
                  ,m_output()
                  ,m_num()
                  ,m_denum()
  {}


  void setCoefficients(  Array<T,N> f_num
                            ,Array<T,M> f_denum)
  {
    m_num.setCoefficients(f_num);
    m_denum.setCoefficients(f_denum);
  }


  float calculating(float f_input){
    float l_output=0;
    float l_numitor=calculatingNumitor(f_input);
    float l_denumitor=calculatingDenumitor();
    l_output=(l_numitor-l_denumitor)/m_denum[0];
    m_output<<l_output;
    return l_output;
  }

protected:
  float calculatingNumitor(float f_input){
    float l_result=0;
    for(uint16_t i=1;i<N;++i){
      l_result+=m_input[i-1]*m_num[i];
    }
    l_result+=f_input*m_num[0];
    m_input<<f_input;
    return l_result;
  }

  float calculatingDenumitor(){
    float l_result=0;
    for(uint16_t i=1;i<M;++i){
      l_result+=m_output[i-1]*m_denum[i];
    }
    return l_result;
  }

  CProcessMemories<T,N-1> m_input;
  CProcessMemories<T,M-1> m_output;
  MyMath::CPolynomialFunction<T,N-1> m_num;
  MyMath::CPolynomialFunction<T,M-1> m_denum;
};

#endif
