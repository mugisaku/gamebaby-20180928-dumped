#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
widget::
notify_flag(uint32_t  v) noexcept
{
  set_flag(v);

    if(m_parent)
    {
      m_parent->notify_flag(v);
    }
}


void
widget::
need_to_redraw() noexcept
{
    if(m_root && !is_needed_to_redraw())
    {
      set_flag(flags::needed_to_redraw);

      m_root->push_widget_that_needed_to_redraw(*this);
    }
}


void
widget::
need_to_reform() noexcept
{
  notify_flag(flags::needed_to_reform);
}


bool
widget::
test_by_point(int  x, int  y) const noexcept
{
  return((x >= m_absolute_point.x) &&
         (y >= m_absolute_point.y) &&
         (x <  (m_absolute_point.x+m_width )) &&
         (y <  (m_absolute_point.y+m_height)));
}


void
widget::
reform(point  base_pt) noexcept
{
  m_absolute_point = base_pt+m_relative_point;

  unset_flag(flags::needed_to_reform);
}


void
widget::
redraw(image&  img) noexcept
{
  render(image_cursor(img,m_absolute_point));

  unset_flag(flags::needed_to_redraw);
}


void
widget::
reform_if_needed(point  base_pt) noexcept
{
    if(test_flag(flags::needed_to_reform))
    {
      reform(base_pt);
    }
}


void
widget::
print(int  indent) const noexcept
{
  printf("%p %s(%s) - flags:%d w:%4d h:%4d rel_pt{%4d,%4d} abs_pt{%4d,%4d}",this,m_name.data(),get_widget_name(),
    m_flags,
    m_width,
    m_height,
    m_relative_point.x,
    m_relative_point.y,
    m_absolute_point.x,
    m_absolute_point.y
  );
}


}}




