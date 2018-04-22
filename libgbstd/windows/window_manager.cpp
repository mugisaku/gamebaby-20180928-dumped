#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




void
window_manager::
clear() noexcept
{
  m_needing_to_refresh =  true;
  m_moving             = false;
  m_touched            = false;

  m_bottom  = nullptr;
  m_top     = nullptr;
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

      m_top->unset_active_flag();
    }

  else
    {
      m_bottom = w;

      m_bottom->set_low(nullptr);
    }


  m_top = w;

  m_top->set_active_flag();

  m_top->set_high(nullptr);

  m_needing_to_refresh =  true;
  m_moving             = false;

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

  m_moving  = false;
  m_touched = false;

  m_needing_to_refresh = true;

  return w;
}


window*
window_manager::
scan_other_windows_than_top(point  pt) noexcept
{
  auto  current = m_top->get_low();

    while(current)
    {
      current->reform();

        if(current->test_by_point(pt.x,pt.y))
        {
          return current;
        }


      current = current->get_low();
    }


  return nullptr;
}


bool
window_manager::
update() noexcept
{
  auto  pt = ctrl.get_point();

    if(!m_top)
    {
      return false;
    }


  m_top->reform();

    if(m_moving)
    {
        if(ctrl.did_mouse_moved())
        {
          m_top->set_point(m_top->get_point()-(m_gripping_point-pt));

          m_gripping_point = pt;

          m_needing_to_refresh = true;

        }


        if(!ctrl.is_mouse_lbutton_pressed())
        {
          m_moving  = false;
          m_touched = false;
        }


      return true;
    }


  auto  cursor_is_inside_of_top = m_top->test_by_point(pt.x,pt.y);

    if(!cursor_is_inside_of_top)
    {
        if(m_touched && !ctrl.is_mouse_lbutton_pressed())
        {
          m_top->get_root().cancel();

          m_touched = false;

          return true;
        }

      else
        if(ctrl.is_mouse_lbutton_pressed())
        {
          auto  new_top = scan_other_windows_than_top(pt);

            if(new_top)
            {
              m_top->unset_active_flag();
              new_top->set_active_flag();

                if(new_top == m_bottom)
                {
                  m_bottom = m_bottom->get_high();
                }


              m_top->set_high(remove(new_top));

              new_top->set_low(m_top)         ;
                               m_top = new_top;
            }

          else
            {
              return false;
            }
        }

      else
        {
          return false;
        }
    }


    if(m_touched)
    {
        if(!ctrl.is_mouse_lbutton_pressed())
        {
          m_touched = false;
        }


      m_top->react();

      return true;
    }

  else
    {
        if(ctrl.is_mouse_lbutton_pressed())
        {
          m_touched = true;

            if(m_top->is_movable() && (pt.y < (m_top->get_point().y+16)))
            {
              m_moving = true;

              m_gripping_point = pt;
            }

          else
            {
              m_top->react();

              return true;
            }
        }
    }


  return false;
}


bool
window_manager::
composite(image&  dst) noexcept
{
  auto  current = m_bottom;

  bool  refreshed = m_needing_to_refresh;

    if(m_needing_to_refresh)
    {
      dst.fill();

      m_needing_to_refresh = false;
    }


    while(current)
    {
      current->reform();

      auto&  style = current->is_active()?   m_active_window_style
                                         : m_inactive_window_style;

        if(current->update_image(style) || refreshed)
        {
          auto  w = current->get_width();
          auto  h = current->get_height();

          images::transfer(current->get_image(),point(),w,h,image_cursor(dst,current->get_point()));

          refreshed = true;
        }


      current = current->get_high();
    }


  return refreshed;
}


void
window_manager::
print() const noexcept
{
  printf("%c %c\n",m_touched? 't':' ',m_moving? 'm':' ');
}




}}




