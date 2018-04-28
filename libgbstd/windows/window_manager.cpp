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
  w->get_root().put_down();

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
put_down_window_all() const noexcept
{
  auto  current = m_bottom;

    while(current)
    {
      current->get_root().put_down();

      current = current->get_high();
    }
}


void
window_manager::
update(const control_device&  condev) noexcept
{
  m_time = condev.time;

    if(!m_top)
    {
      return;
    }


  m_top->update();


  m_old_mouse = m_mouse               ;
                m_mouse = condev.mouse;

    if(is_moving_window())
    {
        if(m_old_mouse.point != m_mouse.point)
        {
          m_top->set_point(m_top->get_point()-(m_gripping_point-m_mouse.point));

          m_gripping_point = m_mouse.point;

          need_to_refresh();
        }


        if(!m_mouse.left_button)
        {
          unset_flag(flags::moving_window);
        }




      return;
    }


  auto  cursor_is_inside_of_top = m_top->test_by_point(m_mouse->x,m_mouse->y);

    if(!cursor_is_inside_of_top)
    {
        if(is_touched_window() && !m_mouse.left_button)
        {
          m_top->get_root().cancel();

          unset_flag(flags::touched_window);

          return;
        }

      else
        if(m_mouse.left_button)
        {
          auto  new_top = scan_other_windows_than_top(m_mouse.point);

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
        if(!m_mouse.left_button)
        {
          unset_flag(flags::touched_window);
        }


      m_top->react(condev);
    }

  else
    {
        if(m_mouse.left_button)
        {
          set_flag(flags::touched_window);

            if(m_top->is_movable() && (m_mouse->y < (m_top->get_point().y+16)))
            {
              set_flag(flags::moving_window);

              m_gripping_point = m_mouse.point;

              return;
            }
        }


      m_top->react(condev);
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

          current->redraw_frame(dst);
          current->redraw_content(dst);

          current = current->get_high();
        }


      unset_flag(flags::needed_to_refresh);

      return true;
    }

  else
    {
        while(current != m_top)
        {
          current->update();

            if(current->is_needed_to_redraw())
            {
                while(current)
                {
                  current->redraw_frame(dst);
                  current->redraw_content(dst);

                  current = current->get_high();
                }


              return true;
            }


          current = current->get_high();
        }


      m_top->update();

        if(m_top->is_needed_to_redraw())
        {
          m_top->redraw_content(dst);

          return true;
        }
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




