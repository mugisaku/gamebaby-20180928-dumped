#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




constexpr int     margin = 16;
constexpr int  thickness =  4;


color  frame::m_default_line_color = predefined_color::yellow;

background_style  frame::m_default_background_style = background_style(color(2,2,2),color(3,3,3),4);


void
frame::
set_line_color(color  new_color) noexcept
{
  m_line_color = new_color;

  need_to_redraw();
}


void
frame::
set_background_style(background_style  new_style) noexcept
{
  m_background_style = new_style;

  need_to_redraw();
}


void
frame::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

    if(m_target)
    {
      m_target->set_relative_point(point(margin,margin));

      m_target->reform_if_needed(get_absolute_point());
 

      auto  rel_pt = m_target->get_relative_point();

      m_width  = rel_pt.x+m_target->get_width() +margin;
      m_height = rel_pt.y+m_target->get_height()+margin;
    }

  else
    {
      m_width  = 1;
      m_height = 1;
    }
}


void
frame::
render(image_cursor  cur) noexcept
{
  cur.draw_text(m_text,m_text_style,margin,0);

  int  x = margin/2;
  int  y = margin/2;
  int  w = m_width -margin;
  int  h = m_height-margin;

  auto  interval = m_background_style.get_interval();

    if(interval)
    {
        for(;;)
        {
            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  goto FINISH;
                }


              cur.draw_hline(m_background_style.get_first_color(),x,y++,w);
            }


            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  goto FINISH;
                }


              cur.draw_hline(m_background_style.get_second_color(),x,y++,w);
            }
        }
    }



FINISH:
  node::render(cur);
}




}}




