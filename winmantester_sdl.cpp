#include<SDL.h>
#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"libgbstd/window.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#include<SDL.h>
#endif


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


gbstd::image
image(screen_w,screen_h);


gbstd::windows::window_manager
winman;


gbstd::widgets::text_roll*
txtrol;


bool
table[6][6] = {0};


gbstd::widgets::menu_parameter
menu_para
{
  6,6,32,32,
    [](gbstd::point  index)->bool
    {
        if(gbstd::ctrl.is_mouse_lbutton_pressed())
        {
          table[index.y][index.x] = true;

          return true;
        }

      else
        if(gbstd::ctrl.is_mouse_rbutton_pressed())
        {
          table[index.y][index.x] = false;

          return true;
        }

      return false;
    },

    [](gbstd::image_cursor  cur, gbstd::point  index)
    {
      auto  b = table[index.y][index.x];

      cur.draw_rectangle(b? gbstd::predefined::red:gbstd::predefined::green,0,0,32,32);
    }
};


void
main_loop()
{
    if(winman.composite(image) || gbstd::ctrl.is_needed_to_redraw())
    {
      sdl::update_screen(image);
    }


  sdl::update_controller();

  static uint32_t  next;

  auto  now = gbstd::ctrl.get_time();

    if(now >= next)
    {
      next = now+40;

        if(txtrol->is_needing_to_linefeed())
        {
          txtrol->linefeed();
        }

      else
        {
          txtrol->type();
        }
    }


    if(gbstd::ctrl.did_mouse_acted())
    {
        if(!winman.update() && gbstd::ctrl.is_mouse_rbutton_pressed())
        {
          auto  pt = gbstd::ctrl.get_point();

          auto  wp = winman.append(new gbstd::window,pt.x,pt.y);
        }
    }
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);

  auto  wp = winman.append(new gbstd::window,200,32);

  (*wp)->append_child(new gbstd::widgets::button(new gbstd::widgets::label(u"NEW"),[](gbstd::widgets::button&  btn){
      if(btn.is_released() && btn.get_count())
      {
        btn.reset_count();

        static int  n;

        auto  wp = winman.append(new gbstd::window(),n,n);

        wp->set_header_flag();
        wp->set_movable_flag();

        txtrol->get_queue().push("あたらしい　ウィンドウが　さくせい　されました");

        n +=    8;
        n &= 0xFF;

        auto  new_btn = new gbstd::widgets::button(new gbstd::widgets::label(u"CLOSE"),[](gbstd::widgets::button&  btn){
            if(btn.is_released() && btn.get_count())
            {
              txtrol->get_queue().push("ウィンドウが　はき　されました");

              btn.reset_count();

              delete winman.remove(btn.get_window());
            }
        });

        (*wp)->append_child(new_btn,0,0);

        (*wp)->show_all();
      }
  }),0,0);


  (*wp)->append_child(new gbstd::widgets::menu(menu_para),80,60);

  txtrol = new gbstd::widgets::text_roll(14,3);

  (*wp)->append_child(txtrol,0,40);

  (*wp)->show_all();

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




