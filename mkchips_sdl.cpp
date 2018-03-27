#include"sdl.hpp"
#include"libgbstd/widget.hpp"



namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 480;


gbstd::image
image(screen_w,screen_h);


gbstd::widgets::root
root;


gbstd::images::color_index
current_color;


void
update_color() noexcept;


gbstd::widgets::dial  r_dial(0,7,[](gbstd::widgets::dial&  d, int  old_value, int  new_value){update_color();});
gbstd::widgets::dial  g_dial(0,7,[](gbstd::widgets::dial&  d, int  old_value, int  new_value){update_color();});
gbstd::widgets::dial  b_dial(0,7,[](gbstd::widgets::dial&  d, int  old_value, int  new_value){update_color();});


void
update_color() noexcept
{
  current_color = gbstd::images::color_index(r_dial.get_current(),
                                             g_dial.get_current(),
                                             b_dial.get_current());
}


class
canvas: public gbstd::widgets::widget
{
  static constexpr int  pixel_size = 8;
  static constexpr int  width  = 16;
  static constexpr int  height = 16;

  gbstd::color_index  m_data[height][width]={0};

public:
  canvas() noexcept: widget(pixel_size*width,pixel_size*height){}

  void  do_when_mouse_acted(int  x, int  y) noexcept override
  {
    auto&  dst = m_data[y/pixel_size][x/pixel_size];

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
      for(int  y = 0;  y < height;  ++y){
      for(int  x = 0;  x <  width;  ++x){
        cur.fill_rectangle(m_data[y][x],pixel_size*x,pixel_size*y,pixel_size,pixel_size);
      }}


      for(int  y = 0;  y < height;  ++y)
      {
        cur.draw_hline(gbstd::images::predefined::gray,0,pixel_size*y,pixel_size*height);
      }


      for(int  x = 0;  x < width;  ++x)
      {
        cur.draw_vline(gbstd::images::predefined::gray,pixel_size*x,0,pixel_size*width);
      }
  }

};


class
color_sample: public gbstd::widgets::widget
{
  static constexpr int   size = 64;

public:
  color_sample() noexcept: widget(size,size){}

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(current_color,0,0,size,size);
  }

};


void
main_loop()
{
  sdl::update_controller();

    if(root.react(image))
    {
      sdl::update_screen(image);
    }
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);

  auto  palette = new gbstd::widgets::table_column({
    r_dial.get_widget(),
    g_dial.get_widget(),
    b_dial.get_widget(),
  });

  root.append_child(palette,200,200);
  root.append_child(new canvas(),10,10);

  

  root.show_all();

    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();

  return 0;
}




