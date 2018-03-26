#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
root::
react() noexcept
{
    if(test_flag(flags::needed_to_reform))
    {
      container::reform();

      unset_flag(flags::needed_to_reform);
    }


  auto  pt = ctrl.get_point();

  bool  mouse_acted = false;

    if(m_previous_ctrl.get_point() != pt)
    {
        if(!m_current)
        {
          m_current = scan_by_point(pt.x,pt.y);

            if(m_current)
            {
              m_current->do_when_cursor_got_in();
            }
        }

      else
        {
          mouse_acted = true;

            if(!m_current->test_by_point(pt.x,pt.y))
            {
              m_current->do_when_cursor_got_out();

              m_current = scan_by_point(pt.x,pt.y);

                if(m_current)
                {
                  m_current->do_when_cursor_got_in();
                }
            }
        }
    }

  else
    if(m_current)
    {
      mouse_acted = (( m_previous_ctrl.is_mouse_lbutton_pressed() && !ctrl.is_mouse_lbutton_pressed()) ||
                     (!m_previous_ctrl.is_mouse_lbutton_pressed() &&  ctrl.is_mouse_lbutton_pressed()) ||
                     ( m_previous_ctrl.is_mouse_rbutton_pressed() && !ctrl.is_mouse_rbutton_pressed()) ||
                     (!m_previous_ctrl.is_mouse_rbutton_pressed() &&  ctrl.is_mouse_rbutton_pressed()));
    }


    if(m_current && mouse_acted)
    {
      pt = pt-m_current->get_absolute_point();

      m_current->do_when_mouse_acted(pt.x,pt.y);
    }


  m_previous_ctrl = ctrl;
}




}}




