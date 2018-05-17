#ifndef context_HPP
#define context_HPP


#include"libgbstd/string.hpp"
#include"libgbstd/control_device.hpp"
#include<cstdint>


namespace gbstd{
namespace contexts{


class context_stack;


class
context
{
  context_stack*  m_stack;

  gbstd::string  m_name;

  uint32_t  m_pc=0;

  enum class state{
    stepping,
    stopping,
    finished,
  } m_state=state::stepping;

  int  m_stored_value=0;

public:
  context(                      ) noexcept{}
  context(gbstd::string_view  sv) noexcept: m_name(sv){}

  virtual ~context(){}

  const uint32_t&  get_pc() const noexcept{return m_pc;}

  void  add_pc(int  v=1) noexcept{m_pc += v;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void  jump(uint32_t  v) noexcept{m_pc = v;}

  bool  is_stepping() const noexcept{return m_state == state::stepping;}
  bool  is_stopping() const noexcept{return m_state == state::stopping;}
  bool  is_finished() const noexcept{return m_state == state::finished;}

  void   stop() noexcept{m_state = state::stopping;}
  void  start() noexcept{m_state = state::stepping;}

  void  sleep(uint32_t  ms) const noexcept;

  void  reset(context_stack&  stack) noexcept;

  void  finish(int  v=0) noexcept;

  int    load(      ) const noexcept{return m_stored_value    ;}
  void  store(int  v)       noexcept{       m_stored_value = v;}

  virtual void  do_when_be_popped() noexcept{}

  virtual void  step(const control_device&  ctrl) noexcept{}

};


class
context_stack
{
  std::vector<context*>  m_data;

  context*  m_top=nullptr;

  uint32_t  m_current_time;
  uint32_t    m_start_time;

  bool  m_sleeping=false;

public:
  void  push(context&  ctx) noexcept;

  void  set_sleep_timer(uint32_t  ms) noexcept;

  void  touch(control_device&  ctrl) noexcept;

};


}


using contexts::context;
using contexts::context_stack;


}




#endif




