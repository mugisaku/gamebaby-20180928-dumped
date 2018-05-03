#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




constexpr int     margin = 16;
constexpr int  thickness =  4;


text_style
frame::
m_default_text_style(
  colors::null,
  colors::white,
  colors::null,
  colors::black
);


color  frame::m_default_line_color = colors::yellow;


void
frame::
set_line_color(color  new_color) noexcept
{
  m_line_color = new_color;

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
  node::render(cur);

  cur.draw_text(m_text,m_text_style,margin,0);

  cur.draw_rectangle(m_line_color,0,0,m_width,m_height);
}




}}




