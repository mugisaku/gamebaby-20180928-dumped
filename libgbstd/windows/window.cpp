#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




window::
window() noexcept
{
  m_root->set_relative_point(point(8,8));

  m_root->set_window(this);
}




bool
window::
test_by_point(int  x, int  y) const noexcept
{
  return((x >= (m_point.x             )) &&
         (y >= (m_point.y             )) &&
         (x <  (m_point.x+get_width() )) &&
         (y <  (m_point.y+get_height())));
}


void
window::
change_state(uint32_t  st) noexcept
{
  m_state = st;

  m_root->set_relative_point(point(8,(m_state&flags::header)? 16:8));

  m_root->need_to_reform();
}


void
window::
react() noexcept
{
  m_root.react(m_point);
}


bool
window::
update() noexcept
{
    if(m_root->is_needed_to_redraw())
    {
      m_root->reform_if_needed(point());

      int  w = m_root->get_width() +16;
      int  h = m_root->get_height()+((m_state&flags::header)? 24:16);

        if((w != get_width()) ||
           (h != get_height()))
        {
          get_image().resize(w,h);

          draw_frame();
        }


      m_root->redraw(get_image());

      return true;
    }


  return false;
}




}}




