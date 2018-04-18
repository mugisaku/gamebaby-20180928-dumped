#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




const icon**
check_button::
get_icons() const noexcept
{
  static const icon*
  icons[] =
  {
    &predefined_icon::radio_checked,
    &predefined_icon::radio_unchecked,
  };


  return icons;
}


void
check_button::
change_state() noexcept
{
  uint32_t  new_state = get_state();

    if(is_checked())
    {
      new_state &= ~m_bit_id;

      m_icons->set_current(0);
    }

  else
    {
      new_state |= m_bit_id;

      m_icons->set_current(1);
    }



  need_to_redraw();

  call(new_state);
}




}}




