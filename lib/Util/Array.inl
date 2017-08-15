

template<typename T,uint32_t N>
typename Array<T,N>::ref Array<T,N>::operator[](Array<T,N>::size_type index){
  return m_elems[index];
}

template<typename T,uint32_t N>
typename Array<T,N>::const_ref Array<T,N>::operator[](Array<T,N>::size_type index) const{
  return m_elems[index];
}


template<typename T,uint32_t N>
typename Array<T,N>::ref Array<T,N>::at(Array<T,N>::size_type index){
  return m_elems[index];
}

template<typename T,uint32_t N>
typename Array<T,N>::const_ref Array<T,N>::at(Array<T,N>::size_type index) const{
  return m_elems[index];
}

template<typename T,uint32_t N>
typename Array<T,N>::ref Array<T,N>::front(){
  return m_elems[0];
}

template<typename T,uint32_t N>
typename Array<T,N>::const_ref Array<T,N>::front() const{
  return m_elems[0];
}

template<typename T,uint32_t N>
typename Array<T,N>::ref Array<T,N>::back(){
  return m_elems[N-1];
}

template<typename T,uint32_t N>
typename Array<T,N>::const_ref Array<T,N>::back() const{
  return m_elems[N-1];
}
