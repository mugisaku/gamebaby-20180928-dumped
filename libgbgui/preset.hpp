#ifndef gbgui_preset_hpp_was_included
#define gbgui_preset_hpp_was_included


#include"libgbgui/canvas.hpp"


namespace gbgui{
namespace presets{


using namespace gbstd;


struct
graphics_editor
{
  image  m_source_image;

  int  m_cell_width;
  int  m_cell_height;

  int  m_table_width;
  int  m_table_height;

  point  m_current_index;

  int  m_table_offset=0;

  canvases::canvas*  m_canvas;
  widgets::frame*   m_canvas_frame;

  widgets::frame*   m_cell_table_frame;

  widgets::color_maker*   m_color_maker;
  widgets::frame*         m_color_maker_frame;
  widgets::color_holder*  m_color_holder;
  widgets::frame*         m_color_holder_frame;

  widgets::frame*  m_tool_widget_frame;
  widgets::frame*  m_operation_widget_frame;

  widget*  m_bg_change_buttons;

  widgets::label*  m_cursor_label;
  widgets::label*  m_table_offset_label;

  widgets::button*  m_save_button;

  widgets::menu*  m_menu;


  background_style  m_bg_style;

  gbstd::string  m_filepath;

  void  (*m_callback)();

  void  load(const std::vector<uint8_t>&  bin) noexcept;

};


graphics_editor*  create_graphics_editor(int  cell_w, int  cell_h, int  table_w, int  table_h) noexcept;

	 
}


}


#endif




