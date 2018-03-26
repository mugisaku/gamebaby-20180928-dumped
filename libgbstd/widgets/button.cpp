#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
button::
do_when_cursor_got_out() noexcept
{
  m_state = state::released;

  need_to_redraw();
}


void
button::
do_when_mouse_acted(int  x, int  y) noexcept
{
    if(is_released())
    {
        if(ctrl.is_mouse_lbutton_pressed())
        {
          m_state = state::pressed;

          need_to_redraw();
        }
    }

  else
    {
        if(!ctrl.is_mouse_lbutton_pressed())
        {
          m_state = state::released;

            if(m_callback)
            {
              m_callback(*this);
            }


          do_when_cursor_got_out();
        }
    }
}




}}




