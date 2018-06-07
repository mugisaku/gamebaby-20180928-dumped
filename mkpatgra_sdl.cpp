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
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  auto&  cont = condev.dropped_file_content;

    if(cont.size())
    {
      ge->load(cont);

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


  ge = presets::create_graphics_editor(24,24,6,2);

  pv.reset();

  ge->m_callback = [](){pv.need_to_redraw();};

  auto  coloring_widget = new widgets::table_column({ge->m_color_maker_frame,ge->m_bg_change_buttons});

  auto  right = new widgets::table_column({ge->m_tool_widget_frame,ge->m_operation_widget_frame,});

  auto  left_under = new widgets::table_row({new widgets::frame(&pv,"preview"),ge->m_cell_table_frame});
  auto        left = new widgets::table_column({ge->m_canvas_frame,left_under,ge->m_save_button});

  root.set_node_target(new widgets::table_row({left,ge->m_color_holder_frame,coloring_widget,right}));



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


  sdl::quit();
#endif


  return 0;
}




