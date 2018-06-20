#ifndef program_HPP
#define program_HPP


#include"libgbstd/string.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/control_device.hpp"
#include<new>
#include<utility>
#include<cstdint>


namespace gbstd{
namespace programs{


class program;


class
value
{
  enum class kind{
    null,
    integer,
    string,
    real_number,
    keyboard,

  } m_kind=kind::null;

  union data{
    int            i;
    gbstd::string  s;
    double         r;
    keyboard     kbd;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(int  i) noexcept{*this = i;}
  value(gbstd::string_view  sv) noexcept{*this = sv;}
  value(keyboard  kbd)      noexcept{*this = kbd;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(int  i) noexcept;
  value&  operator=(gbstd::string_view  sv) noexcept;
  value&  operator=(keyboard  kbd) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_integer() const noexcept{return m_kind == kind::integer;}
  bool  is_string()  const noexcept{return m_kind == kind::string;}
  bool  is_keyboard() const noexcept{return m_kind == kind::keyboard;}

  int                   get_integer() const noexcept{return m_data.i;}
  const gbstd::string&  get_string()  const noexcept{return m_data.s;}
  keyboard              get_keyboard()  const noexcept{return m_data.kbd;}

};


class
context
{
  program*  m_program;

  gbstd::string  m_name;

  uint32_t  m_pc;

  uint32_t  m_calling_count;

  value  m_end_value;

  bool  m_ended;
  bool  m_removed;

public:
  context(                      ) noexcept{}
  context(gbstd::string_view  sv) noexcept: m_name(sv){}

  virtual ~context(){}

  bool  is_ended()   const noexcept{return m_ended;}
  bool  is_removed() const noexcept{return m_removed;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}

  void  set_pc(int  v  ) noexcept{m_pc  = v;}
  void  add_pc(int  v=1) noexcept{m_pc += v;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void  reset(programs::program&  program) noexcept;

  void   set_end_value(value&&  v) noexcept{m_end_value = std::move(v);}
  value  get_end_value(          ) noexcept{return std::move(m_end_value);}

  void  call(context&  ctx) noexcept;

  void  end(value  v=value()) noexcept;

  void  remove() noexcept;

  void  ready_to_step() noexcept{m_calling_count = 0;}

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

  operator bool() const noexcept{return m_top;}

  void  push(context&  ctx) noexcept;

  void  pop(value  v=value()) noexcept;

  void  step() noexcept;

};




}}




#endif




