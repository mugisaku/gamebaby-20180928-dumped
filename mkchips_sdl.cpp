#include"sdl.hpp"
#include"libgbstd/widget.hpp"



namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 480;


gbstd::image
image(screen_w,screen_h);


gbstd::widgets::root
root;


class
canvas: public gbstd::widgets::widget
{
  static constexpr int  pixel_size = 8;
  static constexpr int  width  = 12;
  static constexpr int  height = 24;

  uint8_t  m_data[height][width]={0};

public:
  canvas() noexcept: widget(pixel_size*width,pixel_size*height){}

  void  do_when_mouse_acted(  int  x, int  y) noexcept override
  {
      if(gbstd::ctrl.is_mouse_lbutton_pressed())
      {
        m_data[y/pixel_size][x/pixel_size] = 1;
        need_to_redraw();
      }
  }

  void  render(gbstd::image_cursor  cur) noexcept override
  {
      for(int  y = 0;  y < height;  ++y){
      for(int  x = 0;  x <  width;  ++x){
        cur.fill_rectangle(m_data[y][x]? gbstd::predefined::white:gbstd::predefined::red,pixel_size*x,pixel_size*y,pixel_size,pixel_size);
     }}
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

  root.append_child(new canvas(),200,200);

  auto  col = new gbstd::widgets::table_column(32,{
    new gbstd::widgets::button(u"R",[](gbstd::widgets::button&  btn){}),
    new gbstd::widgets::button(u"G",[](gbstd::widgets::button&  btn){}),
    new gbstd::widgets::button(u"B",[](gbstd::widgets::button&  btn){}),
  });


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




