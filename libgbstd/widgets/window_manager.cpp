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
      auto  high = current->m_high;

      delete current       ;
             current = high;
    }


  m_modified_flag =  true;
  m_moving_flag   = false;

  m_bottom = nullptr;
  m_top    = nullptr;
}


window_pointer
window_manager::
new_window(int  x, int  y) noexcept
{
  auto  win = new window(x,y);

  win->m_manager = this;

    if(m_top)
    {
      m_top->m_high = win;

      win->m_low = m_top;
    }

  else
    {
      m_bottom = win;
    }


  m_modified_flag =  true;
  m_moving_flag   = false;

  m_top = win;

  return window_pointer(*win);
}


void
window_manager::
delete_window(window_pointer  ptr) noexcept
{
  remove(*ptr);

  m_modified_flag =  true;
  m_moving_flag   = false;

    if(m_top == ptr.m_data)
    {
      m_top    = nullptr;
      m_bottom = nullptr;
    }


  delete ptr.m_data;
}


void
window_manager::
remove(window&  win) noexcept
{
    if(win.m_high)
    {
      win.m_high->m_low = win.m_low;
    }


    if(win.m_low)
    {
      win.m_low->m_high = win.m_high;
    }
}


void
window_manager::
reset_windows_all() noexcept
{
  auto  current = m_bottom;

    while(current)
    {
      current->update();

      current = current->m_high;
    }


  m_modified_flag =  true;
  m_moving_flag   = false;
}


bool
window_manager::
is_any_window_modified() noexcept
{
    if(m_modified_flag)
    {
      m_modified_flag = false;

      return true;
    }


  return false;
}


void
window_manager::
touch(window&  win) noexcept
{
  win.react();

    if(win.is_image_modified())
    {
      m_modified_flag = true;
    }

  else
    if(ctrl.is_mouse_lbutton_pressed() && !win.get_current())
    {
      m_moving_flag = true;

      m_gripping_point = ctrl.get_point();
    }
}


void
window_manager::
update() noexcept
{
  auto  pt = ctrl.get_point();

  auto  current = m_top;

    if(current)
    {
        if(m_moving_flag)
        {
            if(ctrl.did_mouse_moved())
            {
report;
              m_modified_flag = true;
            }


            if(!ctrl.is_mouse_lbutton_pressed())
            {
report;
              m_moving_flag = false;
            }
        }

      else
        if(current->test_by_point(pt.x,pt.y))
        {
          touch(*current);
        }

      else
        {
          current = current->m_low;

            while(current)
            {
                if(current->test_by_point(pt.x,pt.y))
                {
                  remove(*current);

                  m_top->m_high = current;

                  current->m_low = m_top          ;
                                   m_top = current;

                  touch(*current);

                  break;
                }


              current = current->m_low;
            }
        }
    }
}


void
window_manager::
composite(image&  dst) noexcept
{
  auto  current = m_bottom;

    while(current)
    {
      images::transfer(image_frame(current->m_image),image_cursor(dst,current->get_point()));

      current = current->m_high;
    }
}




}}




