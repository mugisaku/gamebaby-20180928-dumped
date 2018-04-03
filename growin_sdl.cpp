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


namespace types{
class
work
{
  gbstd::string_view  m_name;

public:
  work(gbstd::string_view  name) noexcept: m_name(name){}

  const gbstd::string_view&  get_name() const noexcept{return m_name;}

};


class
status
{
  int  m_physical_level        =0;
  int  m_physical_level_max    =0;
  int  m_mental_level          =0;
  int  m_mental_level_max      =0;
  int  m_intellectual_level    =0;
  int  m_intellectual_level_max=0;

  int  m_power_of_arms=0;
  int  m_power_of_legs=0;
  int  m_power_of_heart=0;
  int  m_power_of_voice=0;

  int  m_cultural_level=0;
  int  m_smile_level   =0;

public:
  status() noexcept{}

  int  get_physical_level()         const noexcept{return m_physical_level        ;}
  int  get_physical_level_max()     const noexcept{return m_physical_level_max    ;}
  int  get_mental_level()           const noexcept{return m_mental_level          ;}
  int  get_mental_level_max()       const noexcept{return m_mental_level_max      ;}
  int  get_intellectual_level()     const noexcept{return m_intellectual_level    ;}
  int  get_intellectual_level_max() const noexcept{return m_intellectual_level_max;}

  int  get_power_of_arms() const noexcept{return m_power_of_arms;}
  int  get_power_of_legs() const noexcept{return m_power_of_legs;}
  int  get_power_of_heart() const noexcept{return m_power_of_heart;}
  int  get_power_of_voice() const noexcept{return m_power_of_voice;}

  int  get_cultural_level() const noexcept{return m_cultural_level;}
  int  get_smile_level() const noexcept{return m_smile_level;}


};


int
post_ad(int&  v, int  n) noexcept
{
  auto  prev_v = v     ;
                 v += n;

  return prev_v;
}


class
status_view: public widgets::widget
{
  const types::status*  m_status;

public:
  status_view(const types::status&  st) noexcept: widget(font_width*16,font_height*12), m_status(&st){}

  void  render(images::image_cursor  cur) noexcept
  {
    gbstd::tmpstr  ts;

    static const images::text_style  style;

    int  y = 0;

    int  h = images::font_height;

    cur.draw_text(ts("　　たいりょく %3d/%3d",m_status->get_physical_level(),m_status->get_physical_level_max()),style,0,post_ad(y,h));
    cur.draw_text(ts("せいしんりょく %3d/%3d",m_status->get_mental_level(),m_status->get_mental_level_max()),style,0,post_ad(y,h));
    cur.draw_text(ts("　　　ちりょく %3d/%3d",m_status->get_intellectual_level(),m_status->get_intellectual_level_max()),style,0,post_ad(y,h*2));

    cur.draw_text(ts("　うでのちから %3d",m_status->get_power_of_arms() ),style,0,post_ad(y,h));
    cur.draw_text(ts("　あしのちから %3d",m_status->get_power_of_legs() ),style,0,post_ad(y,h));
    cur.draw_text(ts("こころのちから %3d",m_status->get_power_of_heart()),style,0,post_ad(y,h));
    cur.draw_text(ts("　こえのちから %3d",m_status->get_power_of_voice()),style,0,post_ad(y,h*2));

    cur.draw_text(ts("きょうよう %3d",m_status->get_cultural_level()),style,0,post_ad(y,h));
    cur.draw_text(ts("　　えがお %3d",m_status->get_smile_level()),style,0,post_ad(y,h));
  }

};


}


types::status
status;


types::status_view*
status_view;


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

  status_view = new types::status_view(status);

  auto  wp = winman.new_window(32,32);

  (*wp)->append_child(status_view,0,0);
  (*wp)->append_child(new widgets::button(new widgets::label(u"CLOSE"),[](widgets::button&  btn){report;}),200,0);

  (*wp)->show_all();

  winman.reset_windows_all();

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




