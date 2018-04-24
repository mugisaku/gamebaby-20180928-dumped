#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




void
window_manager::
clear() noexcept
{
  m_bottom  = nullptr;
  m_top     = nullptr;

  m_state = 0;

  need_to_refresh();
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

  unset_flag(flags::touched_window);
  unset_flag(flags::moving_window);

  need_to_refresh();

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

  unset_flag(flags::touched_window);
  unset_flag(flags::moving_window);

  need_to_refresh();

  return w;
}


window*
window_manager::
scan_other_windows_than_top(point  pt) noexcept
{
  auto  current = m_top->get_low();

    while(current)
    {
      current->update();

        if(current->test_by_point(pt.x,pt.y))
        {
          return current;
        }


      current = current->get_low();
    }


  return nullptr;
}


void
window_manager::
update() noexcept
{
  auto  pt = ctrl.get_point();

    if(!m_top)
    {
      return;
    }


  m_top->update();

    if(is_moving_window())
    {
        if(ctrl.did_mouse_moved())
        {
          m_top->set_point(m_top->get_point()-(m_gripping_point-pt));

          m_gripping_point = pt;

          need_to_refresh();
        }


        if(!ctrl.is_mouse_lbutton_pressed())
        {
          unset_flag(flags::moving_window);
        }


      return;
    }


  auto  cursor_is_inside_of_top = m_top->test_by_point(pt.x,pt.y);

    if(!cursor_is_inside_of_top)
    {
        if(is_touched_window() && !ctrl.is_mouse_lbutton_pressed())
        {
          m_top->get_root().cancel();

          unset_flag(flags::touched_window);

          return;
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

              need_to_refresh();
            }

          else
            {
              return;
            }
        }

      else
        {
          return;
        }
    }


    if(is_touched_window())
    {
        if(!ctrl.is_mouse_lbutton_pressed())
        {
          unset_flag(flags::touched_window);
        }


      m_top->react();

      return;
    }

  else
    {
        if(ctrl.is_mouse_lbutton_pressed())
        {
          set_flag(flags::touched_window);

            if(m_top->is_movable() && (pt.y < (m_top->get_point().y+16)))
            {
              set_flag(flags::moving_window);

              m_gripping_point = pt;
            }

          else
            {
              m_top->react();
            }
        }
    }
}


bool
window_manager::
composite(image&  dst) noexcept
{
  auto  current = m_bottom;

    if(is_needed_to_refresh())
    {
      dst.fill();

        while(current)
        {
          current->update();

          auto&  style = current->is_active()?   m_active_window_style
                                             : m_inactive_window_style;

          current->redraw(style,dst);

          current = current->get_high();
        }


      unset_flag(flags::needed_to_refresh);

      return true;
    }

  else
    {
      bool  flag = false;

        while(current)
        {
            if(current->update())
            {
              flag = true;
            }


            if(flag)
            {
              auto&  style = current->is_active()?   m_active_window_style
                                                 : m_inactive_window_style;

              current->redraw(style,dst);
            }


          current = current->get_high();
        }


      unset_flag(flags::needed_to_refresh);

      return flag;
    }


  return false;
}


void
window_manager::
print() const noexcept
{
//  printf("%c %c\n",m_touched? 't':' ',m_moving? 'm':' ');
}




}}




