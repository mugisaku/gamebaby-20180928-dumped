#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




void
window_manager::
clear() noexcept
{
  m_needing_to_refresh =  true;
  m_moving_flag        = false;

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

      m_bottom->set_low(nullptr);
    }


  m_top = w;

  m_top->set_high(nullptr);

  m_needing_to_refresh =  true;
  m_moving_flag        = false;

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

  m_needing_to_refresh = true;

  return w;
}


void
window_manager::
touch(window&  win) noexcept
{
  win.react();

    if(win.is_movable() && ctrl.is_mouse_lbutton_pressed() && !win.get_current())
    {
      m_moving_flag = true;

      m_gripping_point = ctrl.get_point();
    }
}


bool
window_manager::
check_other_windows_than_top() noexcept
{
  auto  pt = ctrl.get_point();

  auto  current = m_top->get_low();

    while(current)
    {
      current->update();

        if(current->test_by_point(pt.x,pt.y))
        {
          m_needing_to_refresh = true;

            if(current == m_bottom)
            {
              m_bottom = m_bottom->get_high();
            }


          m_top->set_high(remove(current));

          current->set_low(m_top)         ;
                           m_top = current;

          touch(*current);

          return true;
        }


      current = current->get_low();
    }


  return false;
}


bool
window_manager::
update() noexcept
{
  auto  pt = ctrl.get_point();

    if(m_moving_flag)
    {
        if(ctrl.did_mouse_moved())
        {
          m_top->set_point(m_top->get_point()-(m_gripping_point-pt));

          m_gripping_point = pt;

          m_needing_to_refresh = true;
        }


        if(!ctrl.is_mouse_lbutton_pressed())
        {
          m_moving_flag = false;
        }


      return true;
    }


    if(m_top)
    {
      m_top->update();

        if(m_top->test_by_point(pt.x,pt.y))
        {
          touch(*m_top);

          return true;
        }

      else
        if(ctrl.is_mouse_button_modified())
        {
          return check_other_windows_than_top();
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
        if(current->update() || refreshed)
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




}}




