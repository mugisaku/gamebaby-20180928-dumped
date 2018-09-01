#include"sdl.hpp"
#include"libgbstd/utility.hpp"


namespace sdl{


namespace{


void
process_key_down(const SDL_KeyboardEvent&  evt, gbstd::keyboard&  kbd) noexcept
{
  if(!evt.repeat)
  {
      switch(evt.keysym.sym)
      {
    case(SDLK_UP   ): kbd.set_up_button();break;
    case(SDLK_LEFT ): kbd.set_left_button();break;
    case(SDLK_RIGHT): kbd.set_right_button();break;
    case(SDLK_DOWN ): kbd.set_down_button();break;

    case(SDLK_SPACE ): kbd.set_start_button();break;
    case(SDLK_LSHIFT):
    case(SDLK_RSHIFT): kbd.set_shift_button();break;

    case(SDLK_RETURN):
    case(SDLK_z):
        kbd.set_p_button();
        break;
    case(SDLK_RCTRL):
    case(SDLK_LCTRL):
    case(SDLK_x    ):
        kbd.set_n_button();
        break;
    case(SDLK_F1):
//        SDL_SaveBMP(surface,"__SCREEN.bmp");
        break;
      }
  }
}


void
process_key_up(const SDL_KeyboardEvent&  evt, gbstd::keyboard&  kbd) noexcept
{
    switch(evt.keysym.sym)
    {
  case(SDLK_UP   ): kbd.unset_up_button();break;
  case(SDLK_LEFT ): kbd.unset_left_button();break;
  case(SDLK_RIGHT): kbd.unset_right_button();break;
  case(SDLK_DOWN ): kbd.unset_down_button();break;

  case(SDLK_SPACE ): kbd.unset_start_button();break;
  case(SDLK_LSHIFT):
  case(SDLK_RSHIFT): kbd.unset_shift_button();break;

  case(SDLK_RETURN):
  case(SDLK_z     ):
      kbd.unset_p_button();
      break;
  case(SDLK_RCTRL):
  case(SDLK_LCTRL):
  case(SDLK_x    ):
      kbd.unset_n_button();
      break;
    }
}


void
process_mouse_button(const SDL_MouseButtonEvent&  evt, gbstd::mouse&  m, std::vector<gbstd::point>&  pts, bool&  flag) noexcept
{
  m.point = gbstd::point(evt.x,evt.y);

  pts.emplace_back(m.point);

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
process_mouse_motion(const SDL_MouseMotionEvent&  evt, gbstd::mouse&  m, std::vector<gbstd::point>&  pts, bool&  flag) noexcept
{
  m.point = gbstd::point(evt.x,evt.y);

  pts.emplace_back(m.point);

    if(evt.state&SDL_BUTTON_LMASK){m.left_button.press(flag);}
  else                            {m.left_button.release(flag);}

    if(evt.state&SDL_BUTTON_RMASK){m.right_button.press(flag);}
  else                            {m.right_button.release(flag);}
}


#ifdef EMSCRIPTEN
void
try_read_dropped_file(gbstd::control_device&  dev) noexcept
{
    if(dev.dropped_file.empty() && gbstd::get_number_of_dropped_files())
    {
      dev.dropped_file = gbstd::pop_front_dropped_file();
    }
}
#else
void
read_dropped_file(gbstd::control_device&  dev, const char*  filepath) noexcept
{
  auto  f = fopen(filepath,"rb");

    if(f)
    {
      dev.dropped_file.clear();

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          dev.dropped_file.emplace_back(c);
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

  dev.time                    = SDL_GetTicks();
  dev.needed_to_redraw        = false;
  dev.mouse_state_modify_flag = false;

#ifdef EMSCRIPTEN
  try_read_dropped_file(dev);
#endif

  auto&  mf = dev.mouse_state_modify_flag;

  dev.point_list.clear();

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_KEYDOWN): process_key_down(evt.key,dev.keyboard);break;
      case(SDL_KEYUP  ): process_key_up(  evt.key,dev.keyboard);break;
      case(SDL_MOUSEBUTTONUP  ): process_mouse_button(evt.button,dev.mouse,dev.point_list,mf);break;
      case(SDL_MOUSEBUTTONDOWN): process_mouse_button(evt.button,dev.mouse,dev.point_list,mf);break;
      case(SDL_MOUSEMOTION): process_mouse_motion(evt.motion,dev.mouse,dev.point_list,mf);break;
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




