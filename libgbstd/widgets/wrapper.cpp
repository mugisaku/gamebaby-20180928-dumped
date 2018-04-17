#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
wrapper::
do_when_cursor_got_in() noexcept
{
    if(m_target)
    {
        if(m_callback)
        {
          m_callback(*this,event_kind::cursor_got_in,0,0);
        }


      m_target->do_when_cursor_got_in();
    }
}


void
wrapper::
do_when_cursor_got_out() noexcept
{
    if(m_target)
    {
        if(m_callback)
        {
          m_callback(*this,event_kind::cursor_got_out,0,0);
        }


      m_target->do_when_cursor_got_out();
    }
}


void
wrapper::
do_when_mouse_acted(int  x, int  y) noexcept
{
    if(m_target)
    {
        if(m_callback)
        {
          m_callback(*this,event_kind::mouse_acted,x,y);
        }


      m_target->do_when_mouse_acted(x,y);
    }
}




void
wrapper::
set_target(widget*  target) noexcept
{
    if(target)
    {
        if(target->get_parent())
        {
          printf("wrapper set_target error: すでに親がいる\n");

          return;
        }


      target->set_parent(this);

      m_target.reset(target);

      need_to_reform();
    }
}


void
wrapper::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

    if(m_target)
    {
      m_target->reform_if_needed(get_absolute_point());
 

      auto  rel_pt = m_target->get_relative_point();

      m_width  = rel_pt.x+m_target->get_width() ;
      m_height = rel_pt.y+m_target->get_height();
    }

  else
    {
      m_width  = 1;
      m_height = 1;
    }
}


void
wrapper::
render(image_cursor  cur) noexcept
{
    if(m_target)
    {
      m_target->render(cur);
    }
}


void
wrapper::
show_all() noexcept
{
  show();

  m_target->show_all();
}




void
wrapper::
print(int  indent) const noexcept
{
  m_target->print(indent);
}




}}




