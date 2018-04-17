#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"libgbstd/widget.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#include<SDL.h>
#endif


using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


namespace types{
class     canvas;
}


namespace ptrs{
types::canvas*      cv;
}


gbstd::image
image(screen_w,screen_h);


widgets::root
root;


gbstd::widget*
color_sample;


gbstd::images::color
current_color;


bool
need_to_hide_cursors;


void
update_color() noexcept;


widgets::dial*  r_dial;
widgets::dial*  g_dial;
widgets::dial*  b_dial;


void
update_color() noexcept
{
  current_color = gbstd::images::color(r_dial->get_current(),
                                       g_dial->get_current(),
                                       b_dial->get_current());

  color_sample->need_to_redraw();
}


constexpr int  cv_w = 16;
constexpr int  cv_h = 16;


gbstd::image
cv_image(cv_w,cv_h);


namespace types{


class
canvas: public widgets::widget
{
  static constexpr int  pixel_size = 12;


public:
  canvas() noexcept: widget(pixel_size*cv_w,pixel_size*cv_h){}

  void  do_when_mouse_acted(int  x, int  y) noexcept override
  {
    auto&  dst = cv_image.get_pixel(x/pixel_size,y/pixel_size);

      if(gbstd::ctrl.is_mouse_lbutton_pressed())
      {
        dst.color = current_color;

        need_to_redraw();
      }

    else
      if(gbstd::ctrl.is_mouse_rbutton_pressed())
      {
        dst.color = color();

        need_to_redraw();
      }
  }

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(gbstd::images::predefined::blue,0,0,pixel_size*cv_w,pixel_size*cv_h);

      for(int  y = 0;  y < cv_h;  ++y){
      for(int  x = 0;  x < cv_w;  ++x){
        auto&  pix = cv_image.get_pixel(x,y);

          if(pix.color)
          {
            cur.fill_rectangle(pix.color,pixel_size*x,pixel_size*y,pixel_size,pixel_size);
          }
      }}


      for(int  y = 0;  y < cv_h;  ++y)
      {
        cur.draw_hline(gbstd::images::predefined::gray,0,pixel_size*y,pixel_size*cv_w);
      }


      for(int  x = 0;  x < cv_w;  ++x)
      {
        cur.draw_vline(gbstd::images::predefined::gray,pixel_size*x,0,pixel_size*cv_h);
      }


    cur.draw_hline(gbstd::images::predefined::light_gray,0,pixel_size*(cv_h/2),pixel_size*cv_w);
    cur.draw_vline(gbstd::images::predefined::light_gray,pixel_size*(cv_w/2),0,pixel_size*cv_h);
  }

};


class
color_sample: public gbstd::widget
{
  static constexpr int   size = 32;

public:
  color_sample() noexcept: widget(size,size){}

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(current_color,0,0,size,size);
  }

};
}




void
clear_all(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

      cv_image.fill(pixel());


      ptrs::cv->need_to_redraw();
    }
}


void
fill_all(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

      cv_image.fill(current_color);


      ptrs::cv->need_to_redraw();
    }
}


void
save(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

#ifdef EMSCRIPTEN
      need_to_hide_cursors = true;

      ptrs::farm->redraw(image);

      need_to_hide_cursors = false;

      sdl::update_screen(image);


      char  buf[256];

      auto&  pt = ptrs::farm->get_absolute_point();
      auto    w = ptrs::farm->get_width();
      auto    h = ptrs::farm->get_height();

      snprintf(buf,sizeof(buf),
        "  var  src = document.getElementById(\"canvas\");"
        "  var  clp = document.createElement(\"canvas\");"
        "  var  ctx = clp.getContext(\"2d\");"
        "  ctx.drawImage(src,%d,%d,%d,%d,0,0,%d,%d);"
        "  var  img = document.getElementById(\"img\");"
        "  img.src = clp.toDataURL();",pt.x,pt.y,w,h,w,h);


      emscripten_run_script(buf);
#else
        for(int  y = 0;  y < cv_h;  ++y)
        {
            for(int  x = 0;  x < cv_w;  ++x)
            {
              image.get_pixel(x,y).color.print();

              printf(",");
            }


          printf("\n");
        }
#endif
    }
}


void
main_loop()
{
  sdl::update_controller();

  root.react();

    if(root->is_needed_to_redraw())
    {
      root->redraw(image);

      sdl::update_screen(image);
    }
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);

  r_dial = new widgets::dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
  g_dial = new widgets::dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
  b_dial = new widgets::dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});

  color_sample = new types::color_sample;

  auto  pal = new widgets::table_column({
    color_sample,
    new widgets::table_row({new widgets::label(u"[ R ]"),r_dial}),
    new widgets::table_row({new widgets::label(u"[ G ]"),g_dial}),
    new widgets::table_row({new widgets::label(u"[ B ]"),b_dial}),
  });


  ptrs::cv   = new types::canvas;

  auto  clra_btn = new widgets::button(new widgets::label(u"CLEAR ALL"),clear_all);;
  auto  fila_btn = new widgets::button(new widgets::label(u" FILL ALL"),fill_all);;
  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);;

  auto  mcol = new widgets::table_column({pal,clra_btn,fila_btn,save_btn});

  auto  rad_mnu = widgets::create_check_menu({new widgets::label(u"draw dot"),
                                              new widgets::label(u"draw line"),
                                              new widgets::label(u"draw circle"),
                                              new widgets::label(u"draw rectangle"),
                                              new widgets::label(u"fill rectangle"),
                                              new widgets::label(u"fill area"),
                                              });

  auto  row = new widgets::table_row({ptrs::cv,mcol,rad_mnu});

  root->append_child(row,0,0);

  root->show_all();

  update_color();


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


//  sdl::quit();

  return 0;
}




