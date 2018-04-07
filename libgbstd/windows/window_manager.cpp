#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




void
window_manager::
clear() noexcept
{
  auto  current = m_bottom;

    while(current)
    {
      auto  hi = current->get_high();

      delete current     ;
             current = hi;
    }


  m_modified_flag =  true;
  m_moving_flag   = false;

  m_bottom = nullptr;
  m_top    = nullptr;
}


window*
window_manager::
append(window*  w, int  x, int  y) noexcept
{
  w->set_point(point(x,y));

  w->set_manager(this);

    if(m_top)
    {
      m_top->set_high(w);

      w->set_low(m_top);
    }

  else
    {
      m_bottom = w;
    }


  m_top = w;

  m_modified_flag =  true;
  m_moving_flag   = false;

  return w;
}


window*
window_manager::
remove(window*  w) noexcept
{
    if(m_top == w)
    {
      m_top = m_top->get_low();

        if(m_top)
        {
          m_top->set_high(nullptr);
        }
    }

  else
    if(m_bottom == w)
    {
      m_bottom = m_bottom->get_high();

        if(m_bottom)
        {
          m_bottom->set_low(nullptr);
        }
    }

  else
    {
      auto  hi = w->get_high();
      auto  lo = w->get_low();

        if(hi)
        {
          hi->set_low(lo);
        }


        if(lo)
        {
          lo->set_high(hi);
        }
    }


  w->set_high(nullptr);
  w->set_low( nullptr);

  m_modified_flag = true;

  return w;
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
        if(ctrl.is_mouse_button_modified())
        {
          current = current->get_low();

            while(current)
            {
              current->update();

                if(current->test_by_point(pt.x,pt.y))
                {
                  m_modified_flag = true;

                    if(current == m_bottom)
                    {
                      m_bottom = m_bottom->get_high();
                    }


                  m_top->set_high(remove(current));

                  current->set_low(m_top)         ;
                                   m_top = current;

                  touch(*current);

                  break;
                }


              current = current->get_low();
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

      images::transfer(image_frame(current->get_image()),image_cursor(dst,current->get_point()));

      current = current->get_high();
    }
}




}}




