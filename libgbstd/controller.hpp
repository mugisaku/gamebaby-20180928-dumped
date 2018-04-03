#ifndef GMBB_CONTROLLER_HPP
#define GMBB_CONTROLLER_HPP


#include"libgbstd/figures.hpp"
#include<cstdint>
#include<cstdio>


namespace gbstd{
namespace controllers{


class
button_state
{
protected:
  uint32_t  m_flags=0;

public:
  void    set(uint32_t  flag)       noexcept{m_flags |=  flag;}
  void  unset(uint32_t  flag)       noexcept{m_flags &= ~flag;}
  bool   test(uint32_t  flag) const noexcept{return(m_flags&flag);}

  uint32_t  get() const noexcept{return m_flags;}

};


class
controller: public button_state
{
  uint32_t  m_time=0;

  point           m_point;
  point  m_previous_point;

public:
  const point&  get_point() const noexcept{return m_point;}
  const point&  get_previous_point() const noexcept{return m_previous_point;}

  void      set_time(uint32_t  v)       noexcept{       m_time = v;}
  uint32_t  get_time(           ) const noexcept{return m_time    ;}


  static constexpr uint32_t         up_button_flag = 0x001;
  static constexpr uint32_t       left_button_flag = 0x002;
  static constexpr uint32_t      right_button_flag = 0x004;
  static constexpr uint32_t       down_button_flag = 0x008;
  static constexpr uint32_t          p_button_flag = 0x010;
  static constexpr uint32_t          n_button_flag = 0x020;
  static constexpr uint32_t      start_button_flag = 0x040;
  static constexpr uint32_t      shift_button_flag = 0x080;
  static constexpr uint32_t     mouse_lbutton_flag = 0x100;
  static constexpr uint32_t     mouse_rbutton_flag = 0x200;
  static constexpr uint32_t       mouse_acted_flag = 0x400;
  static constexpr uint32_t  needed_to_redraw_flag = 0x800;

  void  save_point() noexcept{m_previous_point = m_point;}

  void  set_point(int  x, int  y) noexcept{m_point = point(x,y);}

  void    press_mouse_lbutton() noexcept;
  void    press_mouse_rbutton() noexcept;
  void  release_mouse_lbutton() noexcept;
  void  release_mouse_rbutton() noexcept;

  bool       is_up_button_pressed() const noexcept{return test(    up_button_flag);}
  bool     is_left_button_pressed() const noexcept{return test(  left_button_flag);}
  bool    is_right_button_pressed() const noexcept{return test( right_button_flag);}
  bool     is_down_button_pressed() const noexcept{return test(  down_button_flag);}
  bool        is_p_button_pressed() const noexcept{return test(     p_button_flag);}
  bool        is_n_button_pressed() const noexcept{return test(     n_button_flag);}
  bool    is_start_button_pressed() const noexcept{return test( start_button_flag);}
  bool    is_shift_button_pressed() const noexcept{return test( shift_button_flag);}
  bool   is_mouse_lbutton_pressed() const noexcept{return test(mouse_lbutton_flag);}
  bool   is_mouse_rbutton_pressed() const noexcept{return test(mouse_rbutton_flag);}
  bool            did_mouse_acted() const noexcept{return test(mouse_acted_flag);}
  bool        is_needed_to_redraw() const noexcept{return test(needed_to_redraw_flag);}

  bool  did_mouse_moved() const noexcept{return m_previous_point != m_point;}


};


extern controller  ctrl;


}


using controllers::ctrl;
using controllers::button_state;
using controllers::controller;


}


#endif




