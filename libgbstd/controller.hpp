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


  struct flags{
    static constexpr uint32_t         pressed_up_button = 0x001;
    static constexpr uint32_t       pressed_left_button = 0x002;
    static constexpr uint32_t      pressed_right_button = 0x004;
    static constexpr uint32_t       pressed_down_button = 0x008;
    static constexpr uint32_t          pressed_p_button = 0x010;
    static constexpr uint32_t          pressed_n_button = 0x020;
    static constexpr uint32_t      pressed_start_button = 0x040;
    static constexpr uint32_t      pressed_shift_button = 0x080;
    static constexpr uint32_t     pressed_mouse_lbutton = 0x100;
    static constexpr uint32_t     pressed_mouse_rbutton = 0x200;
    static constexpr uint32_t     modified_mouse_button = 0x400;
    static constexpr uint32_t          needed_to_redraw = 0x800;
  };


  void  save_point() noexcept{m_previous_point = m_point;}

  void  set_point(int  x, int  y) noexcept{m_point = point(x,y);}

  void    press_mouse_lbutton() noexcept;
  void    press_mouse_rbutton() noexcept;
  void  release_mouse_lbutton() noexcept;
  void  release_mouse_rbutton() noexcept;

  bool       is_up_button_pressed() const noexcept{return test(    flags::pressed_up_button);}
  bool     is_left_button_pressed() const noexcept{return test(  flags::pressed_left_button);}
  bool    is_right_button_pressed() const noexcept{return test( flags::pressed_right_button);}
  bool     is_down_button_pressed() const noexcept{return test(  flags::pressed_down_button);}
  bool        is_p_button_pressed() const noexcept{return test(     flags::pressed_p_button);}
  bool        is_n_button_pressed() const noexcept{return test(     flags::pressed_n_button);}
  bool    is_start_button_pressed() const noexcept{return test( flags::pressed_start_button);}
  bool    is_shift_button_pressed() const noexcept{return test( flags::pressed_shift_button);}
  bool   is_mouse_lbutton_pressed() const noexcept{return test(flags::pressed_mouse_lbutton);}
  bool   is_mouse_rbutton_pressed() const noexcept{return test(flags::pressed_mouse_rbutton);}
  bool   is_mouse_button_modified() const noexcept{return test(flags::modified_mouse_button);}
  bool            did_mouse_acted() const noexcept{return test(flags::modified_mouse_button) || did_mouse_moved();}
  bool        is_needed_to_redraw() const noexcept{return test(flags::needed_to_redraw);}

  bool  did_mouse_moved() const noexcept{return m_previous_point != m_point;}


};


extern controller  ctrl;


}


using controllers::ctrl;
using controllers::button_state;
using controllers::controller;


}


#endif




