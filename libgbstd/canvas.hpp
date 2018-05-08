#ifndef gbstd_canvas_hpp_was_included
#define gbstd_canvas_hpp_was_included


#include"libgbstd/widget.hpp"


namespace gbstd{
namespace canvases{


class
canvas: public widget
{
  image_cursor  m_image_cursor;

  int  m_editing_width ;
  int  m_editing_height;

  int  m_pixel_size=1;

  bool  m_grid=false;

  enum class mode{
    draw_dot,
    draw_line,
    draw_rectangle,
    fill_rectangle,
    fill_area,
    select,
    paste,
    layer,

  } m_mode=mode::draw_dot;


  drawing_recorder  m_recorder;

  bool  m_drawing_is_fixed=true;

  color  m_drawing_color=color(0,0,0);

  image  m_clipped_image;

  int  m_pointing_count=0;

  point  m_a_point;
  point  m_b_point;

  rectangle  m_operation_rect;

  void  (*m_callback)(canvas&  cv)=nullptr;

  void  try_to_push_solid_record()    noexcept;
  void  try_to_push_nonsolid_record() noexcept;

  image  get_temporary_image() const noexcept;

public:
  canvas() noexcept;
  canvas(image&  img, int  w, int  h, void  (*callback)(canvas&  cv)) noexcept;

  void  set_cursor_offset(int  x, int  y) noexcept;

  void  set_editing_size(int  w, int  h) noexcept;

  int  get_editing_width()  const noexcept{return m_editing_width;}
  int  get_editing_height() const noexcept{return m_editing_height;}

  int   get_pixel_size(      ) const noexcept{return m_pixel_size;}
  void  set_pixel_size(int  n)       noexcept;

  void   set_drawing_color(colors::color  color)       noexcept{       m_drawing_color = color;}
  color  get_drawing_color(                    ) const noexcept{return m_drawing_color        ;}

  drawing_recorder&  get_drawing_recorder() noexcept{return m_recorder;}

  void    set_grid() noexcept;
  void  unset_grid() noexcept;

  void  change_mode_to_draw_dot()       noexcept{m_mode = mode::draw_dot;}
  void  change_mode_to_draw_line()      noexcept{m_mode = mode::draw_line;}
  void  change_mode_to_draw_rectangle() noexcept{m_mode = mode::draw_rectangle;}
  void  change_mode_to_fill_rectangle() noexcept{m_mode = mode::fill_rectangle;}
  void  change_mode_to_fill_area()      noexcept{m_mode = mode::fill_area;}
  void  change_mode_to_select()         noexcept{m_mode = mode::select;}
  void  change_mode_to_paste()          noexcept{m_mode = mode::paste;}
  void  change_mode_to_layer()          noexcept{m_mode = mode::layer;}

  void    set_image(image&  img, int  w, int  h)       noexcept;
  image&  get_image(                           ) const noexcept{return m_image_cursor.get_image();}

  void  reform(point  base_pt) noexcept override;

  void  modify_dot(colors::color  new_color, int  x, int  y) noexcept;

  void  revolve() noexcept;
  void  reverse_horizontally() noexcept;
  void  reverse_vertically() noexcept;
  void  mirror_vertically() noexcept;
  void  shift_up(bool  rotate) noexcept;
  void  shift_left(bool  rotate) noexcept;
  void  shift_right(bool  rotate) noexcept;
  void  shift_down(bool  rotate) noexcept;

  void  draw_line(colors::color  color, point  a, point  b) noexcept;
  void  draw_rect(colors::color  color, rectangle  rect) noexcept;
  void  fill_rect(colors::color  color, rectangle  rect) noexcept;
  void  fill_area(colors::color  color, point  pt) noexcept;

  widget*  create_tool_widget() noexcept;
  widget*  create_operation_widget() noexcept;

  void  cancel_drawing() noexcept;
  void  cancel_select() noexcept;

  void  take_copy() noexcept;
  void  paste(point  pt, bool  layer) noexcept;
  void  undo() noexcept;

  void  do_when_cursor_got_out() noexcept override;

  void  update() noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


}


using canvases::canvas;


}


#endif




