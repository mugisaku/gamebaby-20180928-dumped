#include"sdl.hpp"
#include"libgbstd/widget.hpp"



using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 480;


gbstd::image
image(screen_w,screen_h);


widgets::root
root;


gbstd::widget*
color_sample;


gbstd::images::color_index
current_color;


void
update_color() noexcept;


widgets::dial  r_dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
widgets::dial  g_dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
widgets::dial  b_dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});


void
update_color() noexcept
{
  current_color = gbstd::images::color_index(r_dial.get_current(),
                                             g_dial.get_current(),
                                             b_dial.get_current());

  color_sample->need_to_redraw();
}


gbstd::color_index
icon_data[widgets::icon::size][widgets::icon::size];


class
canvas: public widgets::widget
{
  static constexpr int  pixel_size = 16;


public:
  canvas() noexcept: widget(pixel_size*widgets::icon::size,pixel_size*widgets::icon::size){}

  void  do_when_mouse_acted(int  x, int  y) noexcept override
  {
    auto&  dst = icon_data[y/pixel_size][x/pixel_size];

      if(gbstd::ctrl.is_mouse_lbutton_pressed())
      {
        dst = current_color;

        need_to_redraw();
      }

    else
      if(gbstd::ctrl.is_mouse_rbutton_pressed())
      {
        dst = 0;

        need_to_redraw();
      }
  }

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(gbstd::images::predefined::black,0,0,pixel_size*widgets::icon::size,pixel_size*widgets::icon::size);

      for(int  y = 0;  y < widgets::icon::size;  ++y){
      for(int  x = 0;  x < widgets::icon::size;  ++x){
        cur.fill_rectangle(icon_data[y][x],pixel_size*x,pixel_size*y,pixel_size,pixel_size);
      }}


      for(int  y = 0;  y < widgets::icon::size;  ++y)
      {
        cur.draw_hline(gbstd::images::predefined::gray,0,pixel_size*y,pixel_size*widgets::icon::size);
      }


      for(int  x = 0;  x < widgets::icon::size;  ++x)
      {
        cur.draw_vline(gbstd::images::predefined::gray,pixel_size*x,0,pixel_size*widgets::icon::size);
      }


    cur.draw_hline(gbstd::images::predefined::light_gray,0,pixel_size*(widgets::icon::size/2),pixel_size*widgets::icon::size);
    cur.draw_vline(gbstd::images::predefined::light_gray,pixel_size*(widgets::icon::size/2),0,pixel_size*widgets::icon::size);
  }

};


namespace types{
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
save() noexcept
{
    for(int  y = 0;  y < widgets::icon::size;  ++y)
    {
        for(int  x = 0;  x < widgets::icon::size;  ++x)
        {
          auto  v = icon_data[y][x];

          printf("0%4o,",v);
        }


      printf("\n");
    }
}


void
main_loop()
{
  sdl::update_controller();

  root.react();

    if(root.is_needed_to_redraw())
    {
      root.redraw(image);

      sdl::update_screen(image);
    }
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);

  color_sample = new types::color_sample;

  auto  pal = new widgets::table_column({
    color_sample,
    r_dial.get_widget(),
    g_dial.get_widget(),
    b_dial.get_widget(),
  });


  auto  cv = new canvas;

  auto  save_btn = new widgets::button(u"SAVE",[](widgets::button&  btn){save();});

  auto  row = new widgets::table_row({cv,pal});
  auto  col = new widgets::table_column({row,save_btn});

  root.append_child(col,0,0);

  root.show_all();

    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();

  return 0;
}




