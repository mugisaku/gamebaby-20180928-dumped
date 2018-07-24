#include"libgbgui/widget.hpp"



namespace gbgui{
namespace widgets{




constexpr int  margin = 16;


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

  m_width  = margin;
  m_height = margin;


  auto  target_width = 0;

    if(m_target)
    {
      m_target->set_relative_point(point(margin,margin));

      m_target->reform_if_needed(get_absolute_point());
 

      auto  rel_pt = m_target->get_relative_point();

      target_width = rel_pt.x+m_target->get_width();

      m_height += rel_pt.y+m_target->get_height();
    }


  auto  l = font_width*u8slen(m_text);

    if(target_width < l)
    {
      target_width = l;
    }


  m_width += target_width;
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




