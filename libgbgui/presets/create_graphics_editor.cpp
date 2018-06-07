#include"libgbgui/preset.hpp"




namespace gbgui{
namespace presets{




rectangle
get_rect(const graphics_editor*  ge, point  index) noexcept
{
   return rectangle(ge->m_cell_width *index.x,
                    ge->m_cell_height*index.y,
                    ge->m_cell_width,
                    ge->m_cell_height);
}




void
update_table_offset_label(const graphics_editor*  ge) noexcept
{
  gbstd::string_form  sf;

  int  num = ge->m_table_offset/ge->m_table_height;
  int  den = ge->m_source_image.get_height()/(ge->m_cell_height*ge->m_table_height);

  ge->m_table_offset_label->set_text(sf("%2d/%2d",num+1,den));
}


void
build_for_coloring(graphics_editor*  ge) noexcept
{
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


  ge->m_color_holder = new widgets::color_holder(color_list,[](widgets::color_holder&  holder, colors::color  color){
    auto  ge = static_cast<graphics_editor*>(holder.get_userdata());
    
    ge->m_color_maker->set_color(color);
  });


  ge->m_color_maker = new widgets::color_maker([](widgets::color_maker&  maker, colors::color  color){
    auto  ge = static_cast<graphics_editor*>(maker.get_userdata());


    ge->m_color_holder->set_color(color);

    ge->m_canvas->set_drawing_color(color);
  });


  ge->m_color_maker_frame  = new widgets::frame(ge->m_color_maker ,"color");
  ge->m_color_holder_frame = new widgets::frame(ge->m_color_holder,"palette");
}


void
build_cell_table(graphics_editor*  ge) noexcept
{
  widgets::menu_item_parameter  mip = {0,0,
    [](widgets::menu&  menu, point  index, mouse_button  left, mouse_button  right)
    {
      auto  ge = static_cast<graphics_editor*>(menu.get_userdata());

        if(left)
        {
          ge->m_current_index = point(0,ge->m_table_offset)+index;

          ge->m_canvas->set_cursor_offset(ge->m_cell_width *ge->m_current_index.x,
                                          ge->m_cell_height*ge->m_current_index.y);

          menu.need_to_redraw();

          ge->m_callback();
        }
    },

    [](widgets::menu&  menu, point  index, image_cursor  cur)
    {
      auto  ge = static_cast<graphics_editor*>(menu.get_userdata());

      auto  i = point(0,ge->m_table_offset)+index;

      auto  src_rect = get_rect(ge,i);

      images::overlay(ge->m_source_image,src_rect,cur);

        if(i == ge->m_current_index)
        {
          cur.draw_rectangle(colors::white,0,0,ge->m_cell_width,
                                               ge->m_cell_height);
        }
    }
  };


  ge->m_menu = new widgets::menu(mip,ge->m_table_width,ge->m_table_height);

  ge->m_menu->set_userdata(ge);

  static const icons::icon*    up_ico[] = {&icons::up};
  static const icons::icon*  down_ico[] = {&icons::down};
  static const icons::icon*  plus_ico[] = {&icons::plus};

  ge->m_table_offset_label = new widgets::label(u" 1/ 1",styles::a_white_based_text_style);

  auto  up_btn = new widgets::button(new widgets::icon_selector(up_ico),[](widgets::button&  btn){
    auto  ge = static_cast<graphics_editor*>(btn.get_userdata());

      if(btn.is_pressed())
      {
          if(ge->m_table_offset >= ge->m_table_height)
          {
            ge->m_table_offset -= ge->m_table_height;

            update_table_offset_label(ge);

            ge->m_menu->need_to_redraw();
          }
      }
  });

  auto  down_btn = new widgets::button(new widgets::icon_selector(down_ico),[](widgets::button&  btn){
    auto  ge = static_cast<graphics_editor*>(btn.get_userdata());

      if(btn.is_pressed())
      {
          if((ge->m_table_offset+ge->m_table_height) < (ge->m_source_image.get_height()/ge->m_cell_height))
          {
            ge->m_table_offset += ge->m_table_height;

            update_table_offset_label(ge);

            ge->m_menu->need_to_redraw();
          }
      }
  });

  auto  ext_btn = new widgets::button(new widgets::icon_selector(plus_ico),[](widgets::button&  btn){
    auto  ge = static_cast<graphics_editor*>(btn.get_userdata());

      if(btn.is_pressed())
      {
        ge->m_source_image.resize(ge->m_source_image.get_width(),
                                  ge->m_source_image.get_height()+(ge->m_cell_height*ge->m_table_height));

        update_table_offset_label(ge);
      }
  });


  auto     btn_col = new widgets::table_column({up_btn,down_btn,ext_btn});
  auto  celtbl_col = new widgets::table_column({ge->m_menu,ge->m_table_offset_label});

  ge->m_cell_table_frame = new widgets::frame(new widgets::table_row({btn_col,celtbl_col}),"cell table");
}



void
build_misc_buttons(graphics_editor*  ge) noexcept
{
  auto  ch1bg_btn = new widgets::button(new widgets::label(u"Change bg1 color"),[](widgets::button&  btn){
    auto  ge = static_cast<graphics_editor*>(btn.get_userdata());

      if(btn.get_count())
      {
        btn.reset_count();

        ge->m_bg_style.set_first_color(ge->m_color_maker->get_color());

        ge->m_canvas->need_to_redraw();
        ge->m_menu->need_to_redraw();
      }
  });

  auto  ch2bg_btn = new widgets::button(new widgets::label(u"Change bg2 color"),[](widgets::button&  btn){
    auto  ge = static_cast<graphics_editor*>(btn.get_userdata());

      if(btn.get_count())
      {
        btn.reset_count();

        ge->m_bg_style.set_second_color(ge->m_color_maker->get_color());

        ge->m_canvas->need_to_redraw();
        ge->m_menu->need_to_redraw();
      }
  });


  ch1bg_btn->set_userdata(ge);
  ch2bg_btn->set_userdata(ge);

  ge->m_bg_change_buttons = new widgets::table_column({new widgets::table_column({ch1bg_btn,ch2bg_btn})});
}



void
build_canvas(graphics_editor*  ge) noexcept
{
  ge->m_cursor_label = new widgets::label(u"X: -- Y: -- PIX: ---",styles::a_white_based_text_style);

  ge->m_canvas = new canvases::canvas(ge->m_source_image,ge->m_cell_width,ge->m_cell_height,[](canvas&  cv, canvas_event  evt){
    auto  ge = static_cast<graphics_editor*>(cv.get_userdata());

      if(evt == canvas_event::image_is_modified)
      {
        ge->m_menu->need_to_redraw();
        ge->m_callback();
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


    ge->m_cursor_label->modify_text(sf.view());
  });


  ge->m_canvas->set_grid();
  ge->m_canvas->set_pixel_size(10);
  ge->m_canvas->set_userdata(ge);


  ge->m_canvas_frame = new widgets::frame(new widgets::table_column({ge->m_canvas,ge->m_cursor_label}),"canvas");

  ge->m_canvas_frame->set_line_color(colors::white);
}


graphics_editor*
create_graphics_editor(int  cell_w, int  cell_h, int  table_w, int  table_h) noexcept
{
  auto  ge = new graphics_editor;

  ge->m_cell_width   = cell_w;
  ge->m_cell_height  = cell_h;
  ge->m_table_width  = table_w;
  ge->m_table_height = table_h;

  build_canvas(ge);
  build_cell_table(ge);

  ge->m_tool_widget_frame      = new widgets::frame(ge->m_canvas->create_tool_widget()     ,"tool");
  ge->m_operation_widget_frame = new widgets::frame(ge->m_canvas->create_operation_widget(),"operation");

  ge->m_canvas->set_cursor_offset(0,0);
  ge->m_canvas->set_editing_size(cell_w,cell_h);

  ge->m_source_image.resize(cell_w*table_w,cell_h*table_h);

  ge->m_menu->set_item_size(cell_w,cell_h);

  ge->m_canvas->set_style(ge->m_bg_style);
  ge->m_menu->set_style(ge->m_bg_style);

  return ge;
}



}}




