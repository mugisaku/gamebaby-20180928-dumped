#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




constexpr int     margin = 16;
constexpr int  thickness =  4;


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
  cur.draw_text(m_text,text_style(),margin,0);

  constexpr color  line_color = predefined_color::yellow;

  cur.draw_hline(line_color,margin/2,         margin/2,m_width-margin);
  cur.draw_hline(line_color,margin/2,m_height-margin/2,m_width-margin);

  cur.draw_vline(line_color,        margin/2,margin/2,m_height-margin);
  cur.draw_vline(line_color,m_width-margin/2,margin/2,m_height-margin);

  wrapper::render(cur);
}




}}




