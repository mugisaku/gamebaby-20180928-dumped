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


widgets::menu*
mnu;


images::image
final_image;


images::image
source_image;


widgets::root
root;


widgets::label*
cursor_label;


widgets::color_maker*
colmak;


widgets::color_holder*
colhol;


background_style
bg_style(color(0,0,4),color(0,0,6),4);


int  cell_width  = 16;
int  cell_height = 16;


rectangle
get_rect(point  index) noexcept
{
   return rectangle(cell_width *index.x,
                    cell_height*index.y,
                    cell_width,
                    cell_height);
}


constexpr int  table_width  = 6;
constexpr int  table_height = 2;


images::point  g_current_index;


namespace{
class
previewer: public gbgui::widget
{
  static constexpr int  m_scale = 3;

public:
  void  reset() noexcept
  {
    set_width( cell_width *m_scale);
    set_height(cell_height*m_scale);

    need_to_reform();
  }

  void  render(image_cursor  cur) noexcept
  {
    rectangle  rect(cell_width,cell_height,
                    cell_width,cell_height);

    rect.x *= g_current_index.x;
    rect.y *= g_current_index.y;

      for(int  y = 0;  y < m_scale;  ++y){
      for(int  x = 0;  x < m_scale;  ++x){
        images::paste(source_image,rect,cur+point(cell_width*x,cell_height*y));
      }}
  }

} pv;
}


void
resize_cell_all(int  w, int  h) noexcept
{
  cell_width  = w;
  cell_height = h;

  cv->set_cursor_offset(0,0);
  cv->set_editing_size(w,h);

  source_image.resize(cell_width*table_width,cell_height*table_height);

  mnu->set_item_size(w,h);


  mnu->need_to_reform();

  pv.reset();
}


void
save() noexcept
{
#ifdef __EMSCRIPTEN__
  auto  buf = source_image.make_image_data();

  transfer_to_javascript(buf.data(),buf.size());

  download_image();
#else
  source_image.save_to_png("__anigra.png");
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
          images::paste(img,img.get_rectangle(),source_image,point());

           cv->need_to_redraw();
          mnu->need_to_redraw();
        }


      cont.resize(0);
    }


  root.react(condev);

    if(root.redraw_only_needed_widgets(final_image) || condev.needed_to_redraw)
    {
      sdl::update_screen(final_image);
    }
}


widget*
create_left_part_widget() noexcept
{
  cursor_label = new widgets::label(u"X: -- Y: -- PIX: ---",styles::a_white_based_text_style);

  cv = new canvas(source_image,cell_width,cell_height,[](canvas&  cv, canvas_event  evt){
      if(evt == canvas_event::image_is_modified)
      {
        mnu->need_to_redraw();
          pv.need_to_redraw();
      }


    string_form  sf;

    auto&  pt = cv.get_current_point();

    auto  color = cv.get_pixel(pt.x,pt.y).color;

      if(color)
      {
        int  r = color.get_r7();
        int  g = color.get_g7();
        int  b = color.get_b7();

        sf("X: %2d Y: %2d PIX: %d%d%d",pt.x,pt.y,r,g,b);
      }

    else
      {
        sf("X: %2d Y: %2d PIX: ---",pt.x,pt.y);
      }


    cursor_label->modify_text(sf.view());
  });


  cv->set_grid();
  cv->set_pixel_size(10);


  auto  cv_frame = new widgets::frame(new widgets::table_column({cv,cursor_label}),"canvas");

  cv_frame->set_line_color(colors::white);

  auto  pv_frame = new widgets::frame(&pv,"preview");

  pv_frame->set_line_color(colors::white);

  return new widgets::table_column({cv_frame,
                                    pv_frame});
}


widget*
create_right_part_widget() noexcept
{
  widgets::menu_item_parameter  mip = {0,0,
    [](widgets::menu&  menu, point  index, mouse_button  left, mouse_button  right)
    {
        if(left)
        {
          g_current_index = index;

          cv->set_cursor_offset(cell_width*index.x,cell_height*index.y);

          menu.need_to_redraw();
            pv.need_to_redraw();
        }
    },

    [](widgets::menu&  menu, point  index, image_cursor  cur)
    {
      auto  src_rect = get_rect(index);

      images::overlay(source_image,src_rect,cur);

        if(index == g_current_index)
        {
          cur.draw_rectangle(colors::white,0,0,cell_width,
                                               cell_height);
        }
    }
  };


  mnu = new widgets::menu(mip,table_width,table_height);


  resize_cell_all(24,24);

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
  });


  colmak = new widgets::color_maker([](widgets::color_maker&  maker, colors::color  color){
    colhol->set_color(color);

    cv->set_drawing_color(color);
  });


  auto  color_maker_frame  = new widgets::frame(colmak,"color");
  auto  color_holder_frame = new widgets::frame(colhol,"palette");


  auto  cv_tool = cv->create_tool_widget();
  auto    cv_op = cv->create_operation_widget();


  auto  save_btn = new widgets::button(new widgets::label(u"Save"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        save();
      }
  });

  auto  ch1bg_btn = new widgets::button(new widgets::label(u"Change bg1 color"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        bg_style.set_first_color(colmak->get_color());

        cv->need_to_redraw();
        mnu->need_to_redraw();
      }
  });

  auto  ch2bg_btn = new widgets::button(new widgets::label(u"Change bg2 color"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        bg_style.set_second_color(colmak->get_color());

        cv->need_to_redraw();
        mnu->need_to_redraw();
      }
  });


  static const icons::icon*    up_ico[] = {&icons::up};
  static const icons::icon*  down_ico[] = {&icons::down};
  static const icons::icon*  plus_ico[] = {&icons::plus};

  auto  up_btn = new widgets::button(new widgets::icon_selector(up_ico),[](widgets::button&  btn){
  });

  auto  down_btn = new widgets::button(new widgets::icon_selector(down_ico),[](widgets::button&  btn){
  });

  auto  ext_btn = new widgets::button(new widgets::icon_selector(plus_ico),[](widgets::button&  btn){
  });

  auto  btn_col = new widgets::table_column({up_btn,down_btn,ext_btn});

  auto  celtbl_frame = new widgets::frame(new widgets::table_row({btn_col,mnu}),"cell table");

  auto  bgchcol = new widgets::table_column({new widgets::table_column({ch1bg_btn,ch2bg_btn})});
  auto  btn_row = new widgets::table_row({bgchcol,save_btn});

  auto  urow = new widgets::table_row({cv_tool,cv_op});
  auto  drow = new widgets::table_row({color_maker_frame});

  auto  ucol = new widgets::table_column({urow,drow,celtbl_frame});

  auto  up_part = new widgets::table_row({color_holder_frame,ucol});

  return new widgets::table_column({up_part,btn_row});
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("mkanigra - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*PNGファイルをドラッグ・アンド・ドロップで読み込む"
                  "</pre>"
  );
#endif

  root.set_node_target(new widgets::table_row({create_left_part_widget(),create_right_part_widget()}));

  auto&  root_node = root.get_node();


  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();

  cv->set_style(bg_style);
  mnu->set_style(bg_style);

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




