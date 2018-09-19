#ifndef ControlDevice_hpp_was_included
#define ControlDevice_hpp_was_included


#include"libgbstd/figures.hpp"
#include"libgbstd/string.hpp"
#include<cstdint>
#include<cstdio>


namespace gbstd{
namespace control_devices{


class
button
{
  bool  m_state=false;

  uint32_t  m_counter=0;

public:
  void  press(bool&  flag) noexcept
  {
      if(!m_state)
      {
        m_state = true;

        flag = true;
      }
  }


  void  release(bool&  flag) noexcept
  {
      if(m_state)
      {
        m_state = false;

        flag = true;

        ++m_counter;
      }
  }

  operator bool() const noexcept{return m_state;}

  uint32_t  get_counter() const noexcept{return m_counter;}
  void    reset_counter(uint32_t  v=0) noexcept{m_counter = v;}

};


class
keyboard
{
  uint32_t  m_state;

  struct flags{
    static constexpr int     up = 0x01;
    static constexpr int   left = 0x02;
    static constexpr int  right = 0x04;
    static constexpr int   down = 0x08;
    static constexpr int      p = 0x10;
    static constexpr int      n = 0x20;
    static constexpr int  shift = 0x40;
    static constexpr int  start = 0x80;
  };

  bool   test(int  flag) const noexcept{return m_state&flag;}

  void    set(int  flag) noexcept{m_state |=  flag;}
  void  unset(int  flag) noexcept{m_state &= ~flag;}

public:
  keyboard(uint32_t  state=0) noexcept: m_state(state){}

  keyboard  operator^(const keyboard&  rhs) const noexcept{return keyboard(m_state^rhs.m_state);}

  uint32_t  get_code() noexcept{return m_state;}

  bool    test_all_button() const noexcept{return m_state;}
  bool     test_up_button() const noexcept{return test(flags::up);}
  bool   test_left_button() const noexcept{return test(flags::left);}
  bool  test_right_button() const noexcept{return test(flags::right);}
  bool   test_down_button() const noexcept{return test(flags::down);}

  bool  test_p_button() const noexcept{return test(flags::p);}
  bool  test_n_button() const noexcept{return test(flags::n);}

  bool  test_shift_button() const noexcept{return test(flags::shift);}
  bool  test_start_button() const noexcept{return test(flags::start);}


  void     set_up_button() noexcept{set(flags::up);}
  void   set_left_button() noexcept{set(flags::left);}
  void  set_right_button() noexcept{set(flags::right);}
  void   set_down_button() noexcept{set(flags::down);}

  void  set_p_button() noexcept{set(flags::p);}
  void  set_n_button() noexcept{set(flags::n);}

  void  set_shift_button() noexcept{set(flags::shift);}
  void  set_start_button() noexcept{set(flags::start);}


  void     unset_up_button() noexcept{unset(flags::up);}
  void   unset_left_button() noexcept{unset(flags::left);}
  void  unset_right_button() noexcept{unset(flags::right);}
  void   unset_down_button() noexcept{unset(flags::down);}

  void  unset_p_button() noexcept{unset(flags::p);}
  void  unset_n_button() noexcept{unset(flags::n);}

  void  unset_shift_button() noexcept{unset(flags::shift);}
  void  unset_start_button() noexcept{unset(flags::start);}

};


struct
mouse
{
  images::point  point;

  button   left_button;
  button  right_button;

        images::point*  operator->()       noexcept{return &this->point;}
  const images::point*  operator->() const noexcept{return &this->point;}

        images::point&  operator*()       noexcept{return this->point;}
  const images::point&  operator*() const noexcept{return this->point;}

};


struct
control_device
{
  uint32_t  time=0;

  control_devices::mouse        mouse;
  control_devices::keyboard  keyboard;

  bool  mouse_state_modify_flag;

  std::vector<point>  point_list;

  std::vector<uint8_t>  dropped_file;

  bool  needed_to_redraw;

};


}


using control_devices::control_device;
using control_devices::mouse;
using control_devices::keyboard;
using mouse_button = control_devices::button;


}


#endif




