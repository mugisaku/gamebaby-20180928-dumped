#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




widget::
~widget()
{
    if(m_deleter)
    {
      m_deleter(m_userdata);
    }
}




void
widget::
notify_flag(uint32_t  v) noexcept
{
  set_flag(v);

    if(m_container)
    {
      m_container->notify_flag(v);
    }
}


void
widget::
need_to_redraw() noexcept
{
  notify_flag(flags::needed_to_redraw);
}


void
widget::
need_to_reform() noexcept
{
  notify_flag(flags::needed_to_reform);
  notify_flag(flags::needed_to_redraw);
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


widget*
widget::
erase() noexcept
{
  auto  next = m_next;

    if(m_previous)
    {
      m_previous->m_next = m_next;
    }


    if(m_next)
    {
      m_next->m_previous = m_previous;
    }


  m_container->remove_child(this);

  return next;
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
redraw_if_needed(image&  img) noexcept
{
    if(test_flag(flags::needed_to_redraw))
    {
      redraw(img);
    }
}


void
widget::
print() const noexcept
{
//  static int  n;

  printf("%p w:%4d h:%4d rel_pt{%4d,%4d} abs_pt{%4d,%4d}\n",this,
    m_width,
    m_height,
    m_relative_point.x,
    m_relative_point.y,
    m_absolute_point.x,
    m_absolute_point.y
  );
}


}}




