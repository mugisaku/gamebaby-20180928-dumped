#include"libgbgui/canvas.hpp"
#include"libgbgui/preset.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;
using namespace gbgui;


namespace{


images::image
final_image;


widgets::root
root;


presets::graphics_editor*
ge;


class
previewer: public gbgui::widget
{
  static constexpr int  m_scale = 3;

public:
  void  reset() noexcept
  {
    set_width( ge->m_cell_width *m_scale);
    set_height(ge->m_cell_height*m_scale);

    need_to_reform();
  }

  void  render(image_cursor  cur) noexcept
  {
    rectangle  rect(ge->m_cell_width,ge->m_cell_height,
                    ge->m_cell_width,ge->m_cell_height);

    rect.x *= ge->m_current_index.x;
    rect.y *= ge->m_current_index.y;

      for(int  y = 0;  y < m_scale;  ++y){
      for(int  x = 0;  x < m_scale;  ++x){
        images::paste(ge->m_source_image,rect,cur+point(ge->m_cell_width*x,ge->m_cell_height*y));
      }}
  }

} pv;


void
save() noexcept
{
#ifdef __EMSCRIPTEN__
  auto  buf = ge->m_source_image.make_image_data();

  transfer_to_javascript(buf.data(),buf.size());

  download_image();
#else
  ge->m_source_image.save_to_png("__anigra.png");
#endif
}


void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

    if(condev.dropped_file_content.size())
    {
      auto&  cont = condev.dropped_file_content;

      images::image  img;

        if(is_image_data(cont.data()))
        {
          img.load_from_image_data(cont.data());
        }

      else
        if(is_png(cont.data()))
        {
          img.load_from_png(cont.data(),cont.size());
        }

      else
        if(is_webp(cont.data()))
        {
          img.load_from_webp(cont.data(),cont.size());
        }


        if(img.get_width())
        {
/*
          int  w = std::max(img.get_width() ,source_image.get_width() );
          int  h = std::max(img.get_height(),source_image.get_height());

          ge->m__table_offset = 0;

          source_image.resize(w,h);

          images::paste(img,img.get_rectangle(),source_image,point());

           cv->need_to_redraw();
          mnu->need_to_redraw();
*/
        }


      cont.resize(0);
    }


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
#ifdef EMSCRIPTEN
  set_caption("mkpatgra - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*PNGファイルをドラッグ・アンド・ドロップで読み込む"
                  "</pre>"
  );
#endif

//  root.set_node_target();

  auto&  root_node = root.get_node();


  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();

  ge = presets::create_graphics_editor(24,24,6,2);

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


  sdl::quit();
#endif


  return 0;
}




