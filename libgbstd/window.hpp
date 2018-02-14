#ifndef GMBB_WINDOW_HPP
#define GMBB_WINDOW_HPP


#include"libgbstd/task.hpp"


namespace gbstd{
namespace windows{


enum class
window_state
{
  hidden,

  full_opened,

  open_to_down,
  open_to_right,
  close_to_up,
  close_to_left,

};


class
window: public task
{
protected:
  window_state  m_state=window_state::full_opened;

  pixel  m_pixels[4] = {pixel(predefined::null),
                        pixel(predefined::blue      ,30000),
                        pixel(predefined::white     ,30000),
                        pixel(predefined::light_gray,30000)};

  int   m_width    =0;
  int   m_width_max=0;
  int  m_height    =0;
  int  m_height_max=0;

  void  draw_frame(image&  dst, point  offset) const noexcept;

public:
  window(              ) noexcept{}
  window(int  w, int  h, point  pt) noexcept: task(pt){resize(w,h);}
  window(rectangle  rect) noexcept: task(rect){resize(rect.w,rect.h);}

  void  resize(int  w, int  h) noexcept;

  bool  operator==(window_state  st) const noexcept{return m_state == st;}
  bool  operator!=(window_state  st) const noexcept{return m_state != st;}

  window_state  get_state(                ) const noexcept{return m_state;}
  void          set_state(window_state  st) noexcept;

  void  change_border0_color(color_index  ci) noexcept;
  void  change_border1_color(color_index  ci) noexcept;
  void  change_surface_color(color_index  ci) noexcept;

  void  animate() noexcept;

  void  render(image&  dst, point  offset) const noexcept override;

};



}


using windows::window;
using windows::window_state;


}




#endif




