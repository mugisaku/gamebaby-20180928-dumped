#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




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
    if(!test_flag(flags::need_to_redraw_self))
    {
      set_flag(flags::need_to_redraw_self);

        if(m_container)
        {
          m_container->notify_flag(flags::need_to_redraw_children);
        }
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
         (x <  m_absolute_end_point.x) &&
         (y <  m_absolute_end_point.y));
}


void
widget::
reform() noexcept
{
  m_absolute_end_point = m_absolute_point+point(m_width,m_height);
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




