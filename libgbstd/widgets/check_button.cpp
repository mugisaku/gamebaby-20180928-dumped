#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
check_button::
change_state() noexcept
{
  uint32_t  new_state = get_state();

    if(is_checked())
    {
      new_state &= ~m_bit_id;
    }

  else
    {
      new_state |= m_bit_id;
    }



  need_to_redraw();

  call(new_state);
}




}}




