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

  m_number_of_windows = 0;
}


window_pointer
window_manager::
new_window(int  x, int  y) noexcept
{
static uint32_t  n;
  auto  win = new window(n++,x,y);
win->set_header_flag();

  win->m_manager = this;

    if(m_top)
    {
      m_top->m_high = win               ;
                      win->m_low = m_top;
    }

  else
    {
      m_bottom = win;
    }


  m_top = win;

  ++m_number_of_windows;

  m_modified_flag =  true;
  m_moving_flag   = false;

  return window_pointer(*win);
}


void
window_manager::
delete_window(window_pointer  ptr) noexcept
{
    if(m_number_of_windows == 1)
    {
      m_top    = nullptr;
      m_bottom = nullptr;
    }

  else
    {
        if(m_top == ptr.m_data)
        {
          m_top = m_top->m_low;

            if(m_top)
            {
              m_top->m_high = nullptr;
            }
        }

      else
        if(m_bottom == ptr.m_data)
        {
          m_bottom = m_bottom->m_high;

            if(m_bottom)
            {
              m_bottom->m_low = nullptr;
            }
        }

      else
        {
          remove(*ptr);
        }
    }


  --m_number_of_windows;
 
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


  win.m_high = nullptr;
  win.m_low  = nullptr;

  m_modified_flag = true;
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
report;
  win.react();
report;

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
    if(!ctrl.did_mouse_acted())
    {
      return;
    }


  auto  pt = ctrl.get_point();

  auto  current = m_top;

    if(current)
    {
      current->update();

        if(m_moving_flag)
        {
            if(ctrl.did_mouse_moved())
            {
              current->set_point(current->get_point()-(m_gripping_point-pt));

              m_gripping_point = pt;

              m_modified_flag = true;
            }


            if(!ctrl.is_mouse_lbutton_pressed())
            {
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
              current->update();

                if(current->test_by_point(pt.x,pt.y))
                {
                  m_modified_flag = true;

                    if(current == m_bottom)
                    {
                      m_bottom = m_bottom->m_high;
                    }


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

  dst.fill();

    while(current)
    {
      current->update();

      images::transfer(image_frame(current->m_image),image_cursor(dst,current->get_point()));

      current = current->m_high;
    }
}




}}




