#ifndef ARRAY_HEADER
#define ARRAY_HEADER

#include <stdint.h>
// #include <cstdio>

#ifdef GTEST_ARRAY
  #include <gtest/gtest.h>
  #include <gtest/gtest_prod.h>
#endif



template<class T,uint32_t N>
struct Array{
  typedef Array<T,N>      this_type;
  typedef T&              ref;
  typedef const T&        const_ref;
  typedef uint32_t        size_type;
  typedef T*              pointer;
  typedef const T*        const_pointer;

  ref                     operator[](size_type index);
  const_ref               operator[](size_type index) const;
  ref                     at(size_type index);
  const_ref               at(size_type index) const;
  ref                     front();
  const_ref               front() const;
  ref                     back();
  const_ref               back() const;

  static this_type        zeros(){
    Array<T,N> l_zeros;
    for(size_type i=0;i<N;++i){
      l_zeros[i]=static_cast<T>(0);
    }
    return l_zeros;
  }

  static this_type        ones(){
    Array<T,N> l_zeros;
    for(size_type i=0;i<N;++i){
      l_zeros[i]=static_cast<T>(1);
    }
    return l_zeros;
  }
  T m_elems[N];
};

#include "Array.inl"

#ifdef GTEST_ARRAY
  #include "../src_test/include/ArrayTest.h"
#endif



#endif
