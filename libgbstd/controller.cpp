#include"controller.hpp"




namespace gbstd{
namespace controllers{


void
controller::
press_mouse_lbutton() noexcept
{
    if(!test(mouse_lbutton_flag))
    {
      set(mouse_lbutton_flag);
      set(mouse_acted_flag);
    }
}


void
controller::
press_mouse_rbutton() noexcept
{
    if(!test(mouse_rbutton_flag))
    {
      set(mouse_rbutton_flag);
      set(mouse_acted_flag);
    }
}


void
controller::
release_mouse_lbutton() noexcept
{
    if(test(mouse_lbutton_flag))
    {
      unset(mouse_lbutton_flag);
        set(mouse_acted_flag);
    }
}


void
controller::
release_mouse_rbutton() noexcept
{
    if(test(mouse_rbutton_flag))
    {
      unset(mouse_rbutton_flag);
        set(mouse_acted_flag);
    }
}




controller
ctrl;


}}




