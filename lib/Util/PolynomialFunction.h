#ifndef POLYNIMIAL_FUNCTION_HEADER
#define POLYNIMIAL_FUNCTION_HEADER

#ifdef GTEST_CPolynomialFunction
  #include <gtest/gtest.h>
  #include <gtest/gtest_prod.h>
#endif

#include <stdint.h>
#include "Array.h"

namespace MyMath{
  uint32_t factorial(uint32_t l_n){
    if(l_n<2){
      return 1;
    }
    uint32_t l_res=1;
    for(uint32_t i=2;i<=l_n;i++){
      l_res*=i;
    }
    return l_res;
  }

  template<uint32_t N>
  Array<float,N+1> binomialCoefficient(){
    if(N==1){
      Array<float,N+1> l_coeff=Array<float,N+1>::ones();
      return l_coeff;
    }
    Array<float,N+1> l_coeff;
    uint32_t l_Nr=(N+1)/2;
    uint32_t l_facN=factorial(N);
    for(uint32_t i=0;i<=l_Nr;++i){
      uint32_t l_facI=factorial(i);
      uint32_t l_facNI=factorial(N-i);
      l_coeff[i]=l_facN/l_facI/l_facNI;
      l_coeff[N-i]=l_coeff[i];
    }
    return l_coeff;
  }

  class CFunction{
    public:
      virtual float operator()(float)=0;
  };

  template<class T,uint32_t N>
  class CPolynomialFunction{
  public:
    typedef CPolynomialFunction<T,N>           this_typ;
    typedef const CPolynomialFunction<T,N>&    const_ref;
    typedef Array<T,N+1>                       coeff_type;
    typedef CPolynomialFunction<T,N-1>         derivate_type;
    typedef Array<T,N>                         derivate_coef_type;


    CPolynomialFunction(const CPolynomialFunction<T,N>& f_polyFnc)
    {
        m_coeff=f_polyFnc.m_coeff;
    }
    CPolynomialFunction(coeff_type f_coeff)
    {
      m_coeff=f_coeff;
    }

    CPolynomialFunction(){
      m_coeff=coeff_type::zeros();
    }

    void setCoefficients(coeff_type f_coeff){
      m_coeff=f_coeff;
    }

    T operator()(T f_input){
      T l_result=static_cast<T>(0.0);
      T l_inputValue=1;
      for(uint32_t i=0;i<N+1;++i){
        l_result+=l_inputValue*m_coeff[i];
        l_inputValue*=f_input;
      }
      return l_result;
    }

    derivate_type derivate(){
      derivate_coef_type l_derCoeff;
      for(uint32_t i=1;i<N+1;i++){
        l_derCoeff[i-1]=m_coeff[i]*i;
      }
      derivate_type l_derivate(l_derCoeff);
      return l_derivate;
    }

    template<class T1,uint32_t M>
    friend class CPolynomialFunction;

    CPolynomialFunction<T,N> operator*(const T& f_value){
        CPolynomialFunction<T,N> l_result(*this);
        for(uint32_t i=0;i<N+1;++i){
          l_result.m_coeff[i]*=f_value;
        }
        return l_result;
    }

    CPolynomialFunction<T,N> operator/(const T& f_value){
        CPolynomialFunction<T,N> l_result(*this);
        for(uint32_t i=0;i<N+1;++i){
          l_result.m_coeff[i]/=f_value;
        }
        return l_result;
    }

    CPolynomialFunction<T,N> operator+(const T& f_value){
        CPolynomialFunction<T,N> l_result(*this);
        for(uint32_t i=0;i<N+1;++i){
          l_result.m_coeff[i]+=f_value;
        }
        return l_result;
    }

    CPolynomialFunction<T,N> operator-(const T& f_value){
        CPolynomialFunction<T,N> l_result(*this);
        for(uint32_t i=0;i<N+1;++i){
          l_result.m_coeff[i]-=f_value;
        }
        return l_result;
    }

    template<uint32_t M>
    CPolynomialFunction<T,(M<N?N:M) > operator+(const CPolynomialFunction<T,M>& f_poliB){
      uint32_t l_length=M<N?N:M;
      CPolynomialFunction<T,(M<N?N:M) > l_result(Array<T,(M<N?N:M)+1>::zeros());
      for(uint32_t i=0;i<l_length+1;++i){
        T l_temp=static_cast<T>(0);
        if(i<N+1){
          l_temp+=m_coeff[i];
        }
        if(i<M+1){
          l_temp+=f_poliB.m_coeff[i];
        }
        l_result.m_coeff[i]=l_temp;
      }
      return l_result;
    }

    template<uint32_t M>
    CPolynomialFunction<T,(M<N?N:M) > operator-(const CPolynomialFunction<T,M>& f_poliB){
      uint32_t l_length=M<N?N:M;
      CPolynomialFunction<T,(M<N?N:M) > l_result(Array<T,(M<N?N:M)+1>::zeros());
      for(uint32_t i=0;i<l_length+1;++i){
        T l_temp=static_cast<T>(0);
        if(i<N+1){
          l_temp+=m_coeff[i];
        }
        if(i<M+1){
          l_temp-=f_poliB.m_coeff[i];
        }
        l_result.m_coeff[i]=l_temp;
      }
      return l_result;
    }

    template<uint32_t M>
    CPolynomialFunction<T,N+M > operator*(const CPolynomialFunction<T,M>& f_poliB){
      //uint32_t l_length=M<N?N:M;
      CPolynomialFunction<T,M+N > l_result(Array<T,M+N+1>::zeros());
      for(uint32_t i=0;i<N+1;++i){
        for(uint32_t j=0;j<M+1;++j){
          l_result.m_coeff[i+j]+=m_coeff[i]*f_poliB.m_coeff[j];
        }
      }
      return l_result;
    }

    coeff_type getCoefficiences(){
      return m_coeff;
    }

    const T operator[](uint32_t f_index){
      return m_coeff[f_index];
    }

  private:
    coeff_type m_coeff;

    #ifdef GTEST_CPolynomialFunction
      FRIEND_TEST(CPolynomialFunction,Initialization);
      FRIEND_TEST(CPolynomialFunction,FunctionDerivate);
      FRIEND_TEST(CPolynomialFunction,OperatorSum);
      FRIEND_TEST(CPolynomialFunction,OperatorSub);
      FRIEND_TEST(CPolynomialFunction,OperatorProduct);
      FRIEND_TEST(CPolynomialFunction,OperatorProdVal);
      FRIEND_TEST(CPolynomialFunction,OperatorDivVal);
      FRIEND_TEST(CPolynomialFunction,OperatorSumVal);
      FRIEND_TEST(CPolynomialFunction,OperatorSubVal);
      FRIEND_TEST(CPolynomialFunction,getCoefficiences);
    #endif
  };

  #ifdef GTEST_CPolynomialFunction
    #include "../src_test/include/PolynomialFunctionTest.h"
  #endif

}
#endif
