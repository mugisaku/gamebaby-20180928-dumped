#include"controller.hpp"




namespace gbstd{
namespace controllers{


void
controller::
press_mouse_lbutton() noexcept
{
    if(!test(flags::pressed_mouse_lbutton))
    {
      set(flags::pressed_mouse_lbutton);
      set(flags::modified_mouse_button);
    }
}


void
controller::
press_mouse_rbutton() noexcept
{
    if(!test(flags::pressed_mouse_rbutton))
    {
      set(flags::pressed_mouse_rbutton);
      set(flags::modified_mouse_button);
    }
}


void
controller::
release_mouse_lbutton() noexcept
{
    if(test(flags::pressed_mouse_lbutton))
    {
      unset(flags::pressed_mouse_lbutton);
        set(flags::modified_mouse_button);
    }
}


void
controller::
release_mouse_rbutton() noexcept
{
    if(test(flags::pressed_mouse_rbutton))
    {
      unset(flags::pressed_mouse_rbutton);
        set(flags::modified_mouse_button);
    }
}




controller
ctrl;


}}




