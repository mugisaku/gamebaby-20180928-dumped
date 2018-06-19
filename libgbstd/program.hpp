#ifndef program_HPP
#define program_HPP


#include"libgbstd/string.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include<new>
#include<utility>
#include<cstdint>


namespace gbstd{
namespace programs{


class
value
{
  enum class kind{
    null,
    integer,
    string,
    real_number,
  } m_kind=kind::null;

  union data{
    int  i;
    gbstd::string  s;
    double  r;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(int  i) noexcept{*this = i;}
  value(gbstd::string_view  sv) noexcept{*this = sv;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(int  i) noexcept;
  value&  operator=(gbstd::string_view  sv) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_integer() const noexcept{return m_kind == kind::integer;}
  bool  is_string()  const noexcept{return m_kind == kind::string;}

  int                   get_integer() const noexcept{return m_data.i;}
  const gbstd::string&  get_string()  const noexcept{return m_data.s;}

};


class
context
{
  gbstd::string  m_name;

  uint32_t  m_pc;

  value  m_value;

  bool  m_halted;
  bool  m_removed;

public:
  context(                      ) noexcept{}
  context(gbstd::string_view  sv) noexcept: m_name(sv){}

  virtual ~context(){}

  bool  is_halted() const noexcept{return m_halted;}
  bool  is_removed() const noexcept{return m_removed;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}

  void  set_pc(int  v  ) noexcept{m_pc  = v;}
  void  add_pc(int  v=1) noexcept{m_pc += v;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void  reset() noexcept;

  void   set_value(value&&  v) noexcept{m_value = std::move(v);}
  value  get_value(          ) noexcept{return std::move(m_value);}

  void  enter(context&  ctx) noexcept;

  void  halt(value  v=value()) noexcept;

  void  remove() noexcept;

  virtual void  do_when_removed() noexcept{}

  virtual void  step() noexcept{}

};


class
program
{
  std::vector<context*>  m_stack;

  context*  m_top=nullptr;

public:
  ~program(){clear();}

  void  clear() noexcept;

  void  push(context&  ctx) noexcept;

  void  pop(value  v=value()) noexcept;

  void  step() noexcept;

};




}}




#endif




