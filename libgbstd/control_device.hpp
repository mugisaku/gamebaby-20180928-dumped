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
      }
  }

  operator bool() const noexcept{return m_state;}

};


struct
keyboard
{
  button     up_button;
  button   left_button;
  button  right_button;
  button   down_button;

  button  p_button;
  button  n_button;

  button  shift_button;
  button  start_button;

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

  bool     mouse_state_modify_flag;
  bool  keyboard_state_modify_flag;

  std::vector<uint8_t>  dropped_file_content;

  bool  needed_to_redraw;

};


}


using control_devices::control_device;
using control_devices::mouse;
using control_devices::keyboard;
using   key_button = control_devices::button;
using mouse_button = control_devices::button;


}


#endif




