#include"libgbgui/canvas.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;
using namespace gbgui;


namespace{


canvas*
cv;


gbstd::image
final_image;


widgets::root
root;


widgets::color_maker*
colmak;


widgets::color_holder*
colhol;


bool
need_to_hide_cursors;


constexpr int  cv_w = 16;
constexpr int  cv_h = 16;


gbstd::image
cv_image(cv_w,cv_h);


void
save(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

#ifdef EMSCRIPTEN
#else
        for(int  y = 0;  y < cv_h;  ++y)
        {
            for(int  x = 0;  x < cv_w;  ++x)
            {
              cv_image.get_pixel(x,y).color.print();

              printf(",");
            }


          printf("\n");
        }


      printf("\n");
#endif
    }
}


void
main_loop()
{
  auto&  condev = sdl::update_control_device();

  root.react(condev);

    if(root.redraw_only_needed_widgets(final_image) || condev.needed_to_redraw)
    {
      sdl::update_screen(final_image);
    }
}




}


int
main(int  argc, char**  argv)
{
  cv = new canvas(cv_image,cv_w,cv_h,nullptr);

  cv->set_grid();
  cv->set_pixel_size(12);


  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);

  auto  color_list = {
    colors::black,
    colors::dark_gray,
    colors::gray,
    colors::light_gray,
    colors::white,
    colors::red,
    colors::green,
    colors::blue,
    colors::yellow,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
  };


  colhol = new widgets::color_holder(color_list,[](widgets::color_holder&  holder, colors::color  color){
    colmak->set_color(color);

    cv->set_drawing_color(color);
  });

  colmak = new widgets::color_maker([](widgets::color_maker&  maker, colors::color  color){
    colhol->set_color(color);

    cv->set_drawing_color(color);
  });


  auto  color_maker_frame  = new widgets::frame(colmak,"color");
  auto  color_holder_frame = new widgets::frame(colhol,"palette");

  auto  mcol = new widgets::table_column({color_maker_frame,cv->create_operation_widget(),save_btn});


  auto cv_frame = new widgets::frame(cv,"canvas");

  root.set_node_target(new widgets::table_row({cv_frame,color_holder_frame,mcol,cv->create_tool_widget()}));


  auto&  root_node = root.get_node();

  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();

  root.redraw(final_image);

  sdl::update_screen(final_image);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  return 0;
}




