#ifndef rw_ptr_TEMPLATE
#define rw_ptr_TEMPLATE


#include<cstddef>
#include<cstdint>


#ifndef report
#define report printf("[report] %s, %s, %d\n",__FILE__,__func__,__LINE__);
#endif


namespace gbstd{


template<typename  T>  class  ro_ptr;
template<typename  T>  class  rw_ptr;


template<typename  T>
class
rw_ptr
{
  T*  data;

  template<typename  U>
  friend class rw_ptr;

public:
  constexpr rw_ptr(       ) noexcept: data(nullptr){}
  constexpr rw_ptr(T*  ptr) noexcept: data(ptr){}

  template<typename  U>
  constexpr rw_ptr(U*  ptr) noexcept: data(static_cast<T*>(ptr)){}

  template<typename  U>
  constexpr rw_ptr(rw_ptr<U>  rhs) noexcept: data(static_cast<T*>(rhs.data)){}

  using value_type = T;

  constexpr value_type*  operator->() const noexcept{return  data;}
  constexpr value_type&  operator *() const noexcept{return *data;}

  constexpr rw_ptr  operator+(int  i) const noexcept{return rw_ptr(data+i);}
  constexpr rw_ptr  operator-(int  i) const noexcept{return rw_ptr(data-i);}

  constexpr ptrdiff_t  operator-(const T*  p) const noexcept{return(data-p);}
  constexpr ptrdiff_t  operator-(rw_ptr  rhs) const noexcept{return(data-rhs.data);}

  constexpr bool  operator==(const T*  p) const noexcept{return data == p;}
  constexpr bool  operator!=(const T*  p) const noexcept{return data != p;}
  constexpr bool  operator< (const T*  p) const noexcept{return data <  p;}
  constexpr bool  operator<=(const T*  p) const noexcept{return data <= p;}
  constexpr bool  operator> (const T*  p) const noexcept{return data >  p;}
  constexpr bool  operator>=(const T*  p) const noexcept{return data >= p;}

  constexpr bool  operator==(rw_ptr  rhs) const noexcept{return data == rhs.data;}
  constexpr bool  operator!=(rw_ptr  rhs) const noexcept{return data != rhs.data;}
  constexpr bool  operator< (rw_ptr  rhs) const noexcept{return data <  rhs.data;}
  constexpr bool  operator<=(rw_ptr  rhs) const noexcept{return data <= rhs.data;}
  constexpr bool  operator> (rw_ptr  rhs) const noexcept{return data >  rhs.data;}
  constexpr bool  operator>=(rw_ptr  rhs) const noexcept{return data >= rhs.data;}

  constexpr bool  operator==(ro_ptr<T>  rhs) const noexcept{return data == rhs.get_raw();}
  constexpr bool  operator!=(ro_ptr<T>  rhs) const noexcept{return data != rhs.get_raw();}
  constexpr bool  operator< (ro_ptr<T>  rhs) const noexcept{return data <  rhs.get_raw();}
  constexpr bool  operator<=(ro_ptr<T>  rhs) const noexcept{return data <= rhs.get_raw();}
  constexpr bool  operator> (ro_ptr<T>  rhs) const noexcept{return data >  rhs.get_raw();}
  constexpr bool  operator>=(ro_ptr<T>  rhs) const noexcept{return data >= rhs.get_raw();}

  constexpr value_type&  operator[](int  i) const noexcept{return data[i];}

  constexpr operator bool() const noexcept{return data;}

  constexpr const T*  get_raw() const noexcept{return data;}
  constexpr void*  get_opaque() const noexcept{return data;}


  rw_ptr&  operator=(T*  p) noexcept
  {
    data = p;

    return *this;
  }

  template<typename  U>
  rw_ptr&  operator=(U*  p) noexcept
  {
    data = static_cast<T*>(p);

    return *this;
  }


  rw_ptr&  operator+=(int  i) noexcept
  {
    data += i;

    return *this;
  }


  rw_ptr&  operator-=(int  i) noexcept
  {
    data -= i;

    return *this;
  }


  rw_ptr&  operator++() noexcept
  {
    ++data;

    return *this;
  }

  rw_ptr&  operator--() noexcept
  {
    --data;

    return *this;
  }


  rw_ptr  operator++(int) noexcept
  {
    auto  p = *this;

    ++data;

    return p;
  }

  rw_ptr  operator--(int) noexcept
  {
    auto  p = *this;

    --data;

    return p;
  }


  constexpr ro_ptr<T>  get_ro() const noexcept{return ro_ptr<T>(data);}

  constexpr uintptr_t  to_uintptr() const noexcept{return reinterpret_cast<uintptr_t>(data);}

};


template<typename  T>
inline rw_ptr<T>
make_rw(T&  t) noexcept
{
  return rw_ptr<T>(&t);
}


}


#endif




