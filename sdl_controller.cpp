#include<SDL.h>
#include"libgbstd/controller.hpp"
#include<cstdlib>



namespace sdl{


namespace{


using flags = gbstd::controller::flags;


void
process_key_down(const SDL_KeyboardEvent&  evt)
{
  if(!evt.repeat)
  {
      switch(evt.keysym.sym)
      {
    case(SDLK_UP   ): gbstd::ctrl.set(flags::pressed_up_button);break;
    case(SDLK_LEFT ): gbstd::ctrl.set(flags::pressed_left_button);break;
    case(SDLK_RIGHT): gbstd::ctrl.set(flags::pressed_right_button);break;
    case(SDLK_DOWN ): gbstd::ctrl.set(flags::pressed_down_button);break;

    case(SDLK_SPACE ): gbstd::ctrl.set(flags::pressed_start_button);break;
    case(SDLK_LSHIFT):
    case(SDLK_RSHIFT): gbstd::ctrl.set(flags::pressed_shift_button);break;

    case(SDLK_RETURN):
    case(SDLK_z):
        gbstd::ctrl.set(flags::pressed_p_button);
        break;
    case(SDLK_RCTRL):
    case(SDLK_LCTRL):
    case(SDLK_x    ):
        gbstd::ctrl.set(flags::pressed_n_button);
        break;
    case(SDLK_F1):
//        SDL_SaveBMP(surface,"__SCREEN.bmp");
        break;
      }
  }
}


void
process_key_up(const SDL_KeyboardEvent&  evt)
{
    switch(evt.keysym.sym)
    {
  case(SDLK_UP   ): gbstd::ctrl.unset(   flags::pressed_up_button);break;
  case(SDLK_LEFT ): gbstd::ctrl.unset( flags::pressed_left_button);break;
  case(SDLK_RIGHT): gbstd::ctrl.unset(flags::pressed_right_button);break;
  case(SDLK_DOWN ): gbstd::ctrl.unset( flags::pressed_down_button);break;

  case(SDLK_SPACE ): gbstd::ctrl.unset(flags::pressed_start_button);break;
  case(SDLK_LSHIFT):
  case(SDLK_RSHIFT): gbstd::ctrl.unset(flags::pressed_shift_button);break;

  case(SDLK_RETURN):
  case(SDLK_z     ):
      gbstd::ctrl.unset(flags::pressed_p_button);
      break;
  case(SDLK_RCTRL):
  case(SDLK_LCTRL):
  case(SDLK_x    ):
      gbstd::ctrl.unset(flags::pressed_n_button);
      break;
    }
}


void
process_mouse_button(const SDL_MouseButtonEvent&  evt)
{
  gbstd::ctrl.set_point(evt.x,evt.y);

    if(evt.state == SDL_PRESSED)
    {
           if(evt.button == SDL_BUTTON_LEFT ){gbstd::ctrl.press_mouse_lbutton();}
      else if(evt.button == SDL_BUTTON_RIGHT){gbstd::ctrl.press_mouse_rbutton();}
    }

  else
    {
           if(evt.button == SDL_BUTTON_LEFT ){gbstd::ctrl.release_mouse_lbutton();}
      else if(evt.button == SDL_BUTTON_RIGHT){gbstd::ctrl.release_mouse_rbutton();}
    }
}


void
process_mouse_motion(const SDL_MouseMotionEvent&  evt)
{
  gbstd::ctrl.set_point(evt.x,evt.y);

    if(evt.state&SDL_BUTTON_LMASK){gbstd::ctrl.press_mouse_lbutton();}
  else                            {gbstd::ctrl.release_mouse_lbutton();}

    if(evt.state&SDL_BUTTON_RMASK){gbstd::ctrl.press_mouse_rbutton();}
  else                            {gbstd::ctrl.release_mouse_rbutton();}
}


}


void
update_controller() noexcept
{
  static SDL_Event  evt;

  gbstd::ctrl.set_time(SDL_GetTicks());
  gbstd::ctrl.save_point();
  gbstd::ctrl.unset(flags::modified_mouse_button);
  gbstd::ctrl.unset(flags::needed_to_redraw);

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_KEYDOWN): process_key_down(evt.key);break;
      case(SDL_KEYUP  ): process_key_up(  evt.key);break;
      case(SDL_MOUSEBUTTONUP  ): process_mouse_button(evt.button);break;
      case(SDL_MOUSEBUTTONDOWN): process_mouse_button(evt.button);break;
      case(SDL_MOUSEMOTION): process_mouse_motion(evt.motion);break;
      case(SDL_WINDOWEVENT):
             switch(evt.window.event)
             {
           case(SDL_WINDOWEVENT_EXPOSED):
               gbstd::ctrl.set(flags::needed_to_redraw);
               break;
             }
           break;
      case(SDL_QUIT):
           quit();
           break;
        }
    }
}


}




