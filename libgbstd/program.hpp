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


class program;


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
  program*  m_program=nullptr;

  gbstd::string  m_name;

  uint32_t  m_pc=0;

  enum class state{
    stepping,
    stopping,
    left,
  } m_state=state::stepping;

public:
  context(                      ) noexcept{}
  context(gbstd::string_view  sv) noexcept: m_name(sv){}

  virtual ~context(){}

  const uint32_t&  get_pc() const noexcept{return m_pc;}

  void  set_pc(int  v  ) noexcept{m_pc  = v;}
  void  add_pc(int  v=1) noexcept{m_pc += v;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  bool  is_stepping() const noexcept{return m_state == state::stepping;}
  bool  is_stopping() const noexcept{return m_state == state::stopping;}
  bool  is_left()     const noexcept{return m_state == state::left;}

  void   stop() noexcept{m_state = state::stopping;}
  void  start() noexcept{m_state = state::stepping;}

  void  sleep(uint32_t  ms) const noexcept;

  void  reset(program&  prog) noexcept;

  value&  get_stored_value() const noexcept;

  void  enter(context&  ctx, void  (*onpop)(context*  ctx)) noexcept;
  void  leave(value  v=value()) noexcept;

  virtual void  step() noexcept{}

};


class
program
{
  struct node;

  node*  m_stack=nullptr;
  node*  m_trash=nullptr;

  context*  m_top=nullptr;

  value  m_stored_value;

  uint32_t  m_current_time;
  uint32_t    m_start_time;

  bool  m_sleeping=false;

  node*  pump_node(         ) noexcept;
  void   dump_node(node*  nd) noexcept;

public:
  ~program(){clear();}

  void  clear() noexcept;

  void  push(context&  ctx, void  (*onpop)(context*  ctx)=nullptr) noexcept;

  void  set_sleep_timer(uint32_t  ms) noexcept;

  void   store_value(value&&  v) noexcept{m_stored_value = std::move(v);}
  value&   get_value() noexcept{return m_stored_value;}

  void  touch(uint32_t  time) noexcept;

};




}}




#endif




