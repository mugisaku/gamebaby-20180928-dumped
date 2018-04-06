#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




constexpr int  margin = 4;


button::
button(widget*  target, void  (*callback)(button&)) noexcept:
m_target(target),
m_callback(callback)
{
}




void
button::
do_when_cursor_got_out() noexcept
{
    if(is_pressed())
    {
      m_state = state::released;

        if(m_callback)
        {
          m_callback(*this);
        }


      need_to_redraw();
    }
}


void
button::
do_when_mouse_acted(int  x, int  y) noexcept
{
    if(is_released())
    {
        if(ctrl.is_mouse_lbutton_pressed())
        {
          m_state = state::pressed;

            if(m_callback)
            {
              m_callback(*this);
            }


          need_to_redraw();
        }
    }

  else
    {
        if(!ctrl.is_mouse_lbutton_pressed())
        {
          ++m_count;

          do_when_cursor_got_out();
        }
    }
}


void
button::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);


  m_target->reform_if_needed(get_absolute_point()+point(margin,margin));

  auto  rel_pt = m_target->get_relative_point();

  m_width  = rel_pt.x+m_target->get_width() +(margin*2);
  m_height = rel_pt.y+m_target->get_height()+(margin*2);
}




void
button::
render(image_cursor  cur) noexcept
{
    if(is_pressed())
    {
      cur.draw_hline(predefined::black,0,         0,m_width);
      cur.draw_hline(predefined::white,0,m_height-1,m_width);
      cur.draw_vline(predefined::black,        0,0,m_height);
      cur.draw_vline(predefined::white,m_width-1,0,m_height);

      cur.fill_rectangle(predefined::gray,1,1,m_width-2,m_height-2);
    }

  else
    {
      cur.draw_hline(predefined::white,0,         0,m_width);
      cur.draw_hline(predefined::black,0,m_height-1,m_width);
      cur.draw_vline(predefined::white,        0,0,m_height);
      cur.draw_vline(predefined::black,m_width-1,0,m_height);

      cur.fill_rectangle(predefined::light_gray,1,1,m_width-2,m_height-2);
    }


    if(m_target)
    {
      m_target->redraw(cur.get_image());
    }
}


void
button::
show_all() noexcept
{
  widget::show();

    if(m_target)
    {
      m_target->show_all();
    }
}


void
button::
print(int  indent) const noexcept
{
  widget::print(indent);

  printf_with_indent(indent+2,"{\n");

    if(m_target)
    {
      m_target->print(indent+2);
    }


  printf_with_indent(indent,"\n}");
}




}}




