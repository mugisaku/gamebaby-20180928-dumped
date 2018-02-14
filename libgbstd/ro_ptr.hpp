#ifndef ro_ptr_TEMPLATE
#define ro_ptr_TEMPLATE


#include"libgbstd/rw_ptr.hpp"


#ifndef report
#define report printf("[report] %s, %s, %d\n",__FILE__,__func__,__LINE__);
#endif


namespace gbstd{


template<typename  T>  class  ro_ptr;
template<typename  T>  class  rw_ptr;


template<typename  T>
class
ro_ptr
{
  const T*  data;

public:
  using value_type = T;

  constexpr ro_ptr(             ) noexcept: data(nullptr){}
  constexpr ro_ptr(const T*  ptr) noexcept: data(ptr){}

  template<typename  U>
  constexpr ro_ptr(const U*  ptr) noexcept: data(static_cast<const T*>(ptr)){}

  constexpr ro_ptr(rw_ptr<T>  rhs) noexcept: data(rhs.get_raw()){}

  template<typename  U>
  constexpr ro_ptr(rw_ptr<U>  rhs) noexcept: data(static_cast<const T*>(rhs.get_raw())){}

  constexpr const value_type*  operator->() const noexcept{return  data;}
  constexpr const value_type&  operator *() const noexcept{return *data;}

  constexpr ro_ptr  operator+(int  i) const noexcept{return ro_ptr(data+i);}
  constexpr ro_ptr  operator-(int  i) const noexcept{return ro_ptr(data-i);}

  constexpr ptrdiff_t  operator-(const T*  p) const noexcept{return(data-p);}
  constexpr ptrdiff_t  operator-(ro_ptr  rhs) const noexcept{return(data-rhs.data);}

  constexpr bool  operator==(const T*  p) const noexcept{return data == p;}
  constexpr bool  operator!=(const T*  p) const noexcept{return data != p;}
  constexpr bool  operator< (const T*  p) const noexcept{return data <  p;}
  constexpr bool  operator<=(const T*  p) const noexcept{return data <= p;}
  constexpr bool  operator> (const T*  p) const noexcept{return data >  p;}
  constexpr bool  operator>=(const T*  p) const noexcept{return data >= p;}

  constexpr bool  operator==(ro_ptr  rhs) const noexcept{return data == rhs.data;}
  constexpr bool  operator!=(ro_ptr  rhs) const noexcept{return data != rhs.data;}
  constexpr bool  operator< (ro_ptr  rhs) const noexcept{return data <  rhs.data;}
  constexpr bool  operator<=(ro_ptr  rhs) const noexcept{return data <= rhs.data;}
  constexpr bool  operator> (ro_ptr  rhs) const noexcept{return data >  rhs.data;}
  constexpr bool  operator>=(ro_ptr  rhs) const noexcept{return data >= rhs.data;}

  constexpr bool  operator==(rw_ptr<T>  rhs) const noexcept{return data == rhs.get_raw();}
  constexpr bool  operator!=(rw_ptr<T>  rhs) const noexcept{return data != rhs.get_raw();}
  constexpr bool  operator< (rw_ptr<T>  rhs) const noexcept{return data <  rhs.get_raw();}
  constexpr bool  operator<=(rw_ptr<T>  rhs) const noexcept{return data <= rhs.get_raw();}
  constexpr bool  operator> (rw_ptr<T>  rhs) const noexcept{return data >  rhs.get_raw();}
  constexpr bool  operator>=(rw_ptr<T>  rhs) const noexcept{return data >= rhs.get_raw();}

  constexpr const value_type&  operator[](int  i) const noexcept{return data[i];}

  constexpr operator bool() const noexcept{return data;}

  constexpr const    T*     get_raw() const noexcept{return data;}
  constexpr const void*  get_opaque() const noexcept{return data;}


  ro_ptr&  operator=(const T*  p) noexcept
  {
    data = p;

    return *this;
  }

  template<typename  U>
  ro_ptr&  operator=(const U*  p) noexcept
  {
    data = static_cast<const T*>(p);

    return *this;
  }


  ro_ptr&  operator+=(int  i) noexcept
  {
    data += i;

    return *this;
  }


  ro_ptr&  operator-=(int  i) noexcept
  {
    data -= i;

    return *this;
  }


  ro_ptr&  operator++() noexcept
  {
    ++data;

    return *this;
  }

  ro_ptr&  operator--() noexcept
  {
    --data;

    return *this;
  }


  ro_ptr  operator++(int) noexcept
  {
    auto  p = *this;

    ++data;

    return p;
  }

  ro_ptr  operator--(int) noexcept
  {
    auto  p = *this;

    --data;

    return p;
  }

  constexpr uintptr_t  to_uintptr() const noexcept{return reinterpret_cast<uintptr_t>(data);}

};


template<typename  T>
inline ro_ptr<T>
make_ro(const T&  t) noexcept
{
  return ro_ptr<T>(&t);
}


inline ro_ptr<char>
make_ro(const char*  s) noexcept
{
  return ro_ptr<char>(s);
}


}


#endif




