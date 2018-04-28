#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
root::
push_widget_that_needed_to_redraw(widget&  w) noexcept
{
    if(m_redrawing_last)
    {
      m_redrawing_last->set_next(&w);
    }

  else
    {
      m_redrawing_first = &w;
    }


  m_redrawing_last = &w;

  w.set_next(nullptr);
}




void
root::
cancel() noexcept
{
    if(m_current)
    {
      m_current->do_when_cursor_got_out();

      m_current = nullptr;
    }
}




void
root::
react(const control_device&  condev) noexcept
{
  m_container.reform_if_needed(point());

  m_old_mouse = m_mouse               ;
                m_mouse = condev.mouse;

  m_mouse.point -= m_offset;

    if(!m_current)
    {
      m_current = m_container.scan_by_absolute_point(m_mouse.point);

        if(m_current)
        {
          m_current->do_when_cursor_got_in();
        }
    }

  else
    if(m_old_mouse.point != m_mouse.point)
    {
        if(!m_current->test_by_absolute_point(m_mouse.point))
        {
            if(m_mouse.left_button)
            {
              auto  abs_pt = m_current->get_absolute_point();

              int    left = abs_pt.x                        ;
              int   right = abs_pt.x+m_current->get_width() ;
              int     top = abs_pt.y                        ;
              int  bottom = abs_pt.y+m_current->get_height();

                   if(m_mouse.point.x <   left){m_mouse.point.x =  left  ;}
              else if(m_mouse.point.x >= right){m_mouse.point.x = right-1;}

                   if(m_mouse.point.y <     top){m_mouse.point.y =    top  ;}
              else if(m_mouse.point.y >= bottom){m_mouse.point.y = bottom-1;}
            }

          else
            {
              m_current->do_when_cursor_got_out();

              m_current = m_container.scan_by_absolute_point(m_mouse.point);

                if(m_current)
                {
                  m_current->do_when_cursor_got_in();
                }
            }
        }
    }


    if(m_current)
    {
      m_current->update();
    }


  m_container.reform_if_needed(point());
}


void
root::
redraw(image&  img) noexcept
{
  m_container.reform_if_needed(point());

  m_container.redraw(img);
}


bool
root::
redraw_only_needed_widgets(image&  img) noexcept
{
  m_container.reform_if_needed(point());

    if(m_redrawing_first)
    {
        while(m_redrawing_first)
        {
          m_redrawing_first->redraw(img);

          m_redrawing_first = m_redrawing_first->get_next();
        }


      m_redrawing_first = nullptr;
      m_redrawing_last  = nullptr;

      return true;
    }


  return false;
}




}}




