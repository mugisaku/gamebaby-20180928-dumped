#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
window_manager::
clear() noexcept
{
  auto  current = m_bottom;

    while(current)
    {
      autp  next = current->m_next;

      delete current       ;
             current = next;
    }


  m_bottom = nullptr;
  m_top    = nullptr;
}


window&
window_manager::
new_window() noexcept
{
  auto  win = new window;

    if(m_top)
    {
      m_top->m_next = win;

      win->m_previous = m_top      ;
    }

  else
    {
      m_bottom = win;
    }


  m_top = win;


  return *win;
}


void
window_manager::
composite(image&  dst) noexcept
{
  auto  current = m_bottom;

    while(current)
    {
      images::trnsfer(image_frame(current->get_image()),image_cursor(dst,current->get_point()));

      current = current->m_next;
    }

}




}}




