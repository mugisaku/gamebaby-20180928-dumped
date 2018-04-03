#include<SDL.h>
#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"sdl_screen.cpp"
#include"sdl_controller.cpp"

#include"libgbstd/widget.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#include<SDL.h>
#endif


using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


gbstd::image
image(screen_w,screen_h);


widgets::window_manager
winman;


void
main_loop()
{
    if(winman.is_any_window_modified())
    {
      winman.composite(image);

      sdl::update_screen(image);
    }

  else
    if(ctrl.is_needed_to_redraw())
    {
      sdl::update_screen(image);
    }


  sdl::update_controller();

  winman.update();
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);


  auto  wp = winman.new_window(32,32);

  (*wp)->append_child(new widgets::button(new widgets::label(u"NEW"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        auto  wp = winman.new_window(0,0);

        auto  new_btn = new widgets::button(new widgets::label(u"CLOSE"),[](widgets::button&  btn){});

        (*wp)->append_child(new_btn,0,0);

        (*wp)->show_all();
      }
  }),0,0);

  (*wp)->show_all();

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  sdl::quit();

  return 0;
}




