#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
root::
react() noexcept
{
  m_container.reform_if_needed(point());

  auto  pt = ctrl.get_point();

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
            if(ctrl.is_mouse_lbutton_pressed())
            {
              auto  abs_pt = m_current->get_absolute_point();

              int    left = abs_pt.x                        ;
              int   right = abs_pt.x+m_current->get_width() ;
              int     top = abs_pt.y                        ;
              int  bottom = abs_pt.y+m_current->get_height();

                   if(pt.x <   left){pt.x =  left  ;}
              else if(pt.x >= right){pt.x = right-1;}

                   if(pt.y <     top){pt.y =    top  ;}
              else if(pt.y >= bottom){pt.y = bottom-1;}
            }

          else
            {
              m_current->do_when_cursor_got_out();

              m_current = m_container.scan_by_point(pt.x,pt.y);

                if(m_current)
                {
                  m_current->do_when_cursor_got_in();
                }
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


bool
root::
update() noexcept
{
    if(m_container.is_needed_to_redraw())
    {
      m_container.reform_if_needed(point());

      return true;
    }


  return false;
}




}}




