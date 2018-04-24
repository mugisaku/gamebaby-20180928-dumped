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
react(point  offset) noexcept
{
  m_container.reform_if_needed(point());

  auto  pt = ctrl.get_point()-offset;

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
      auto  abs_pt = m_current->get_absolute_point();

      pt -= abs_pt;

      m_current->do_when_mouse_acted(pt.x,pt.y);
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




