#ifndef LIBGBSND_OBJECT_HPP
#define LIBGBSND_OBJECT_HPP


#include<cstdint>
#include<cstdio>
#include<memory>
#include"libgbstd/string.hpp"
#include"libgbsnd/shared_string.hpp"
#include"libgbsnd/device.hpp"
#include"libgbsnd/list.hpp"


namespace gbsnd{


class execution_context;


namespace stmts{
class stmt;
class stmt_list;
class routine;
}


namespace objects{




class object;
class routine;
class value;


class
property
{
  void*  m_pointer=nullptr;

  int  (*m_callback)(void*  ptr, const int*  v);

public:
  constexpr property(void*  pointer, int  (*callback)(void*,const int*)) noexcept:
  m_pointer(pointer),
  m_callback(callback){}

  template<typename  T>
  constexpr property(T&  t, int  (*callback)(T*,const int*)) noexcept:
  m_pointer(&t),
  m_callback(reinterpret_cast<int  (*)(void*,const int*)>(callback)){}

  int   get(      ) const noexcept;
  void  set(int  v) const noexcept;

};


class
reference
{
  object*  m_pointer=nullptr;

public:
  reference(object&  o) noexcept: m_pointer(&o){}

  object&  operator()() const noexcept{return *m_pointer;}

  property  get_property(const identifier&  id) const noexcept;

};


struct system{};


class
value
{
  enum class kind{
    null,
    integer,
    reference,
    routine,
    property,
    square_wave,
    noise,
    system,

  } m_kind=kind::null;

  union data{
    int              i;
    reference        r;
    const stmts::routine*  rt;
    property        pr;
    square_wave*    sq;
    noise*          no;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(bool  b) noexcept{*this = b;}
  value(int  i) noexcept{*this = i;}
  value(reference  r) noexcept{*this = r;}
  value(const stmts::routine&  rt) noexcept{*this = rt;}
  value(const property&  pr) noexcept{*this = pr;}
  value(square_wave&  sq) noexcept{*this = sq;}
  value(noise&  no) noexcept{*this = no;}
  value(system  sys) noexcept{*this = sys;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(bool  b) noexcept;
  value&  operator=(int  i) noexcept;
  value&  operator=(reference  r) noexcept;
  value&  operator=(const stmts::routine&  rt) noexcept;
  value&  operator=(const property&  pr) noexcept;
  value&  operator=(square_wave&  sq) noexcept;
  value&  operator=(noise&  no) noexcept;
  value&  operator=(system  sys) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  void  clear() noexcept;

  bool  is_reference()   const noexcept{return m_kind == kind::reference;}
  bool  is_integer()     const noexcept{return m_kind == kind::integer;}
  bool  is_routine()     const noexcept{return m_kind == kind::routine;}
  bool  is_property()    const noexcept{return m_kind == kind::property;}
  bool  is_square_wave() const noexcept{return m_kind == kind::square_wave;}
  bool  is_noise()       const noexcept{return m_kind == kind::noise;}
  bool  is_system()      const noexcept{return m_kind == kind::system;}

  int                get_integer()     const noexcept{return m_data.i;}
  reference          get_reference()   const noexcept{return m_data.r;}
  const stmts::routine&  get_routine()     const noexcept;
  const property&    get_property()    const noexcept{return m_data.pr;}
  square_wave&       get_square_wave() const noexcept{return *m_data.sq;}
  noise&             get_noise()       const noexcept{return *m_data.no;}

  int  get_integer_safely() const noexcept;

  void  print() const noexcept;

};


class
value_list: public list<value>
{
public:
  using list::list;

};


class
object: public value
{
  gbstd::string  m_name;

public:
  object(value&&  v, gbstd::string_view  name) noexcept: value(std::move(v)), m_name(name){}
  object(            gbstd::string_view  name) noexcept:                      m_name(name){}

  void                   set_name(gbstd::string_view  name)       noexcept{m_name = name;}
  const gbstd::string&   get_name(                        ) const noexcept{return m_name;}

  void  print() const noexcept
  {
    printf("%s = ",m_name.data());

    value::print();
  }

};


}


using objects::reference;
using objects::value;
using objects::value_list;
using objects::object;
using objects::property;


}




#endif




