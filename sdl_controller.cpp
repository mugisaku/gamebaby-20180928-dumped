#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>


EM_JS(bool,test_dropped_file,(),
{
  return dropped_file;
});


EM_JS(void,unset_dropped_file,(),
{
  dropped_file = null;
});


EM_JS(uint8_t,get_dropped_file_data,(int  i),
{
  return dropped_file[i];
});


EM_JS(int,get_dropped_file_size,(),
{
  return dropped_file.length;
});


#endif


namespace sdl{


namespace{


void
process_key_down(const SDL_KeyboardEvent&  evt, gbstd::keyboard&  kbd, bool&  flag) noexcept
{
  if(!evt.repeat)
  {
      switch(evt.keysym.sym)
      {
    case(SDLK_UP   ): kbd.up_button.press(flag);break;
    case(SDLK_LEFT ): kbd.left_button.press(flag);break;
    case(SDLK_RIGHT): kbd.right_button.press(flag);break;
    case(SDLK_DOWN ): kbd.down_button.press(flag);break;

    case(SDLK_SPACE ): kbd.start_button.press(flag);break;
    case(SDLK_LSHIFT):
    case(SDLK_RSHIFT): kbd.shift_button.press(flag);break;

    case(SDLK_RETURN):
    case(SDLK_z):
        kbd.p_button.press(flag);
        break;
    case(SDLK_RCTRL):
    case(SDLK_LCTRL):
    case(SDLK_x    ):
        kbd.n_button.press(flag);
        break;
    case(SDLK_F1):
//        SDL_SaveBMP(surface,"__SCREEN.bmp");
        break;
      }
  }
}


void
process_key_up(const SDL_KeyboardEvent&  evt, gbstd::keyboard&  kbd, bool&  flag) noexcept
{
    switch(evt.keysym.sym)
    {
  case(SDLK_UP   ): kbd.up_button.release(flag);break;
  case(SDLK_LEFT ): kbd.left_button.release(flag);break;
  case(SDLK_RIGHT): kbd.right_button.release(flag);break;
  case(SDLK_DOWN ): kbd.down_button.release(flag);break;

  case(SDLK_SPACE ): kbd.start_button.release(flag);break;
  case(SDLK_LSHIFT):
  case(SDLK_RSHIFT): kbd.shift_button.release(flag);break;

  case(SDLK_RETURN):
  case(SDLK_z     ):
      kbd.p_button.release(flag);
      break;
  case(SDLK_RCTRL):
  case(SDLK_LCTRL):
  case(SDLK_x    ):
      kbd.n_button.release(flag);
      break;
    }
}


void
process_mouse_button(const SDL_MouseButtonEvent&  evt, gbstd::mouse&  m, bool&  flag) noexcept
{
  m.point = gbstd::point(evt.x,evt.y);

    if(evt.state == SDL_PRESSED)
    {
           if(evt.button == SDL_BUTTON_LEFT ){ m.left_button.press(flag);}
      else if(evt.button == SDL_BUTTON_RIGHT){m.right_button.press(flag);}
    }

  else
    {
           if(evt.button == SDL_BUTTON_LEFT ){ m.left_button.release(flag);}
      else if(evt.button == SDL_BUTTON_RIGHT){m.right_button.release(flag);}
    }
}


void
process_mouse_motion(const SDL_MouseMotionEvent&  evt, gbstd::mouse&  m, bool&  flag) noexcept
{
  m.point = gbstd::point(evt.x,evt.y);

    if(evt.state&SDL_BUTTON_LMASK){m.left_button.press(flag);}
  else                            {m.left_button.release(flag);}

    if(evt.state&SDL_BUTTON_RMASK){m.right_button.press(flag);}
  else                            {m.right_button.release(flag);}
}


#ifdef EMSCRIPTEN
void
try_read_dropped_file(gbstd::control_device&  dev) noexcept
{
    if(test_dropped_file())
    {
      auto  n = get_dropped_file_size();

      dev.dropped_file_content.resize(n);

        for(int  i = 0;  i < n;  ++i)
        {
          dev.dropped_file_content[i] = get_dropped_file_data(i);
        }


      unset_dropped_file();
    }
}
#else
void
read_dropped_file(gbstd::control_device&  dev, const char*  filepath) noexcept
{
  auto  f = fopen(filepath,"rb");

    if(f)
    {
      dev.dropped_file_content.resize(0);

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          dev.dropped_file_content.emplace_back(c);
        }


      fclose(f);
    }
}
#endif


}


gbstd::control_device&
update_control_device() noexcept
{
  static gbstd::control_device  dev;

  static SDL_Event  evt;

  dev.time                       = SDL_GetTicks();
  dev.needed_to_redraw           = false;
  dev.mouse_state_modify_flag    = false;
  dev.keyboard_state_modify_flag = false;

#ifdef EMSCRIPTEN
  try_read_dropped_file(dev) noexcept;
#endif

  auto&  mf = dev.mouse_state_modify_flag   ;
  auto&  kf = dev.keyboard_state_modify_flag;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_KEYDOWN): process_key_down(evt.key,dev.keyboard,kf);break;
      case(SDL_KEYUP  ): process_key_up(  evt.key,dev.keyboard,kf);break;
      case(SDL_MOUSEBUTTONUP  ): process_mouse_button(evt.button,dev.mouse,mf);break;
      case(SDL_MOUSEBUTTONDOWN): process_mouse_button(evt.button,dev.mouse,mf);break;
      case(SDL_MOUSEMOTION): process_mouse_motion(evt.motion,dev.mouse,mf);break;
      case(SDL_WINDOWEVENT):
             switch(evt.window.event)
             {
           case(SDL_WINDOWEVENT_EXPOSED):
               dev.needed_to_redraw = true;
               break;
             }
           break;
#ifndef EMSCRIPTEN
      case(SDL_DROPFILE):
            read_dropped_file(dev,evt.drop.file);

            SDL_free(evt.drop.file);
            break;
#endif
      case(SDL_QUIT):
           quit();
           break;
        }
    }


  return dev;
}


}




