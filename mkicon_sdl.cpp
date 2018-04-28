#include"libgbstd/image.hpp"
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


widgets::canvas*
cv;


gbstd::image
final_image(screen_w,screen_h);


widgets::root
root;


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
  sdl::init(screen_w,screen_h);


  cv = new widgets::canvas(cv_image,nullptr);

  cv->set_grid();
  cv->set_pixel_size(12);


  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);

  auto  mcol = new widgets::table_column({cv->create_color_maker(),cv->create_operation_widget(),save_btn});

  auto  row = new widgets::table_row({cv,mcol,cv->create_tool_widget()});

  root->append_child(row,0,0);

  root->show_all();

  root.put_down();

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


//  sdl::quit();

  return 0;
}




