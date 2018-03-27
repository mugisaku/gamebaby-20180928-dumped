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
    if(!test_flag(v))
    {
      set_flag(v);

        if(m_container)
        {
          m_container->notify_flag(v);
        }
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
reform(point  abs_pt) noexcept
{
  m_absolute_point = abs_pt;
}


void
widget::
redraw(image&  img) noexcept
{
  render(image_cursor(img,m_absolute_point));
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
print() const noexcept
{
  static int  n;

  printf("%p %8d\n",this,n++);
}


}}




