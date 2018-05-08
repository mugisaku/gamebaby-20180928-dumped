#include"libgbstd/canvas.hpp"



namespace gbstd{
namespace canvases{




namespace{
auto
canvas_background_style = background_style(color(0,0,4),color(0,0,6),4);
}



canvas::
canvas() noexcept
{
  set_style(canvas_background_style);
}


canvas::
canvas(image&  img, int  w, int  h, void  (*callback)(canvas&  cv)) noexcept:
m_callback(callback)
{
  set_style(canvas_background_style);

  set_image(img,w,h);
}




void
canvas::
set_cursor_offset(int  x, int  y) noexcept
{
  m_image_cursor.set_offset(x,y);

  need_to_redraw();
}


void
canvas::
set_pixel_size(int  n) noexcept
{
  m_pixel_size = n;

  need_to_reform();
}


void
canvas::
set_grid() noexcept
{
  m_grid = true;

  need_to_redraw();
}


void
canvas::
unset_grid() noexcept
{
  m_grid = false;

  need_to_redraw();
}




void
canvas::
set_image(image&  img, int  w, int  h) noexcept
{
    if(m_image_cursor)
    {
      cancel_select();
      cancel_drawing();
    }


  m_image_cursor = image_cursor(img);

  m_editing_width  = w? w:img.get_width() ;
  m_editing_height = h? h:img.get_height();

  m_drawing_is_fixed = true;

  m_recorder.clear();

  need_to_redraw();
}


void
canvas::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

  m_operation_rect = rectangle(0,0,m_editing_width,m_editing_height);

  m_width  = m_pixel_size*m_editing_width ;
  m_height = m_pixel_size*m_editing_height;
}


void
canvas::
modify_dot(color  new_color, int  x, int  y) noexcept
{
  auto&  pix = m_image_cursor.get_pixel(x,y);

    if(pix.color != new_color)
    {
      auto&  offset = m_image_cursor.get_offset();

      m_recorder.put(pix.color,offset.x+x,
                               offset.y+y);

      pix.color = new_color;


      need_to_redraw();
    }
}




void
canvas::
cancel_drawing() noexcept
{
  m_pointing_count = 0;

    if(!m_drawing_is_fixed)
    {
        if((m_mode == mode::paste) ||
           (m_mode == mode::layer))
        {
          m_recorder.rollback(m_image_cursor.get_image());
        }


      m_drawing_is_fixed = true;

      need_to_redraw();
    }
}




void
canvas::
try_to_push_solid_record() noexcept
{
    if(m_recorder.push(true))
    {
      m_drawing_is_fixed = false;

      need_to_redraw();

        if(m_callback)
        {
          m_callback(*this);
        }
    }
}


void
canvas::
try_to_push_nonsolid_record() noexcept
{
    if(m_drawing_is_fixed)
    {
      m_recorder.push(false);
    }

  else
    {
      m_drawing_is_fixed = true;
    }
}




void
canvas::
cancel_select() noexcept
{
  m_operation_rect.x = 0;
  m_operation_rect.y = 0;
  m_operation_rect.w = m_editing_width ;
  m_operation_rect.h = m_editing_height;

  need_to_redraw();
}


void
canvas::
do_when_cursor_got_out() noexcept
{
  cancel_drawing();
}


void
canvas::
take_copy() noexcept
{
  m_clipped_image.resize(m_operation_rect.w,m_operation_rect.h);

    for(int  y = 0;  y < m_operation_rect.h;  ++y){
    for(int  x = 0;  x < m_operation_rect.w;  ++x){
      auto  pix = m_image_cursor.get_pixel(m_operation_rect.x+x,m_operation_rect.y+y);

      m_clipped_image.set_pixel(pix,x,y);
    }}
}


void
canvas::
undo() noexcept
{
    if(m_recorder.rollback(m_image_cursor.get_image()))
    {
      need_to_redraw();

        if(m_callback)
        {
          m_callback(*this);
        }
    }
}


image
canvas::
get_temporary_image() const noexcept
{
  image  img(m_editing_width,m_editing_height);

    for(int  y = 0;  y < m_editing_height;  ++y){
    for(int  x = 0;  x < m_editing_width ;  ++x){
      auto  pix = m_image_cursor.get_pixel(x,y);

      img.set_pixel(pix,x,y);
    }}


  return std::move(img);
}


void
canvas::
render(image_cursor  cur) noexcept
{
  const int  w = m_editing_width ;
  const int  h = m_editing_height;

  render_background(cur);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = m_image_cursor.get_pixel(x,y);

        if(pix.color)
        {
          cur.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
        }
    }}


    if(m_grid)
    {
        for(int  y = 0;  y < h;  ++y)
        {
          cur.draw_hline(colors::light_gray,0,m_pixel_size*y,m_width);
        }


        for(int  x = 0;  x < w;  ++x)
        {
          cur.draw_vline(colors::light_gray,m_pixel_size*x,0,m_height);
        }


      cur.draw_hline(colors::white,0,m_pixel_size*(h/2),m_width);
      cur.draw_vline(colors::white,m_pixel_size*(w/2),0,m_height);
    }


  cur.draw_doubleline_rectangle(colors::white,colors::black,
    m_pixel_size*m_operation_rect.x,
    m_pixel_size*m_operation_rect.y,
    m_pixel_size*m_operation_rect.w,
    m_pixel_size*m_operation_rect.h
  );
}




}}




