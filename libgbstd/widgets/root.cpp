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

    if(m_current)
    {
        if(m_previous_ctrl.get_point() != pt)
        {
            if(m_current->test_by_point(pt.x,pt.y))
            {
              m_current->do_when_cursor_moved(pt.x,pt.y);
            }

          else
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
    {
      m_current = scan_by_point(pt.x,pt.y);

        if(m_current)
        {
          m_current->do_when_cursor_got_in();
        }
    }


    if(m_current)
    {
//        if(m_previous_ctrl.is_
    }


  m_previous_ctrl = ctrl;
}




}}




