#include"libgbstd/window.hpp"
#include<cstdlib>




namespace gbstd{
namespace windows{




void
window::
resize(int  w, int  h) noexcept
{
  m_width_max  = w;
  m_height_max = h;

  set_state(window_state::full_opened);
}


void
window::
set_state(window_state  st) noexcept
{
  m_state = st;

    switch(m_state)
    {
  case(window_state::hidden):
      break;
  case(window_state::open_to_down):
      m_width  = m_width_max;
      m_height =          16;
      break;
  case(window_state::close_to_left):
  case(window_state::close_to_up):
  case(window_state::full_opened):
      m_width  = m_width_max;
      m_height = m_height_max;
      break;
  case(window_state::open_to_right):
      m_width  =           16;
      m_height = m_height_max;
      break;
    }
}




void
window::
change_border0_color(color_index  ci) noexcept
{
  m_pixels[2] = ci;
}


void
window::
change_border1_color(color_index  ci) noexcept
{
  m_pixels[2] = ci;
}


void
window::
change_surface_color(color_index  ci) noexcept
{
  m_pixels[1] = ci;
}




void
window::
animate() noexcept
{
  constexpr int  step = 8;

    switch(m_state)
    {
  case(window_state::hidden):
  case(window_state::full_opened):
      break;
  case(window_state::open_to_right):
        if(m_width < m_width_max)
        {
          m_width += step;

            if(m_width >= m_width_max)
            {
              m_width = m_width_max;

              m_state = window_state::full_opened;
            }
        }
      break;
  case(window_state::close_to_left):
        if(m_width > 16)
        {
          m_width -= step;

            if(m_width <= 16)
            {
              m_width = 16;

              m_state = window_state::hidden;
            }
        }
      break;
  case(window_state::open_to_down):
        if(m_height < m_height_max)
        {
          m_height += step;

            if(m_height >= m_height_max)
            {
              m_height = m_height_max;

              m_state = window_state::full_opened;
            }
        }
      break;
  case(window_state::close_to_up):
        if(m_height > 16)
        {
          m_height -= step;

            if(m_height <= 16)
            {
              m_height = 16;

              m_state = window_state::hidden;
            }
        }
      break;
    }
}


void
window::
render(image&  dst, point  offset) const noexcept
{
    if(m_state != window_state::hidden)
    {
      draw_frame(dst,offset);
    }
}




}}




