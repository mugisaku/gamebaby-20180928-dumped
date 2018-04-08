#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




window::
window() noexcept
{
  m_container.set_relative_point(point(8,8));

  m_container.set_window(this);
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


bool
window::
is_image_modified() noexcept
{
    if(m_modified)
    {
      m_modified = false;

      return true;
    }


  return false;
}


void
window::
change_state(uint32_t  st) noexcept
{
  m_state    =   st;
  m_modified = true;

  m_container.set_relative_point(point(8,(m_state&flags::header)? 16:8));

  m_container.need_to_reform();
}


void
window::
react() noexcept
{
  m_container.reform_if_needed(point());

  auto  pt = ctrl.get_point()-m_point;

    if(!m_current)
    {
      m_current = m_container.scan_by_point(pt.x,pt.y);

        if(m_current)
        {
          m_current->do_when_cursor_got_in();
        }
    }

  else
    if(ctrl.did_mouse_moved())
    {
        if(!m_current->test_by_point(pt.x,pt.y))
        {
          m_current->do_when_cursor_got_out();

          m_current = m_container.scan_by_point(pt.x,pt.y);

            if(m_current)
            {
              m_current->do_when_cursor_got_in();
            }
        }
    }


    if(m_current && ctrl.did_mouse_acted())
    {
      pt -= m_current->get_absolute_point();

      m_current->do_when_mouse_acted(pt.x,pt.y);
    }


  update();
}


void
window::
update() noexcept
{
    if(m_container.is_needed_to_redraw())
    {
      m_container.reform_if_needed(point());

      int  w = m_container.get_width() +16;
      int  h = m_container.get_height()+((m_state&flags::header)? 24:16);

        if((w != get_width()) ||
           (h != get_height()))
        {
          get_image().resize(w,h);

          draw_frame();
        }


      m_container.redraw(get_image());

      m_modified = true;
    }
}




}}




