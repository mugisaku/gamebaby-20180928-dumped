#include"libgbstd/canvas.hpp"



namespace gbstd{
namespace canvases{




canvas::background_style
canvas::
m_default_background_style = background_style(color(0,0,4),color(0,0,6),4);




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
set_image(image&  img) noexcept
{
  m_image = &img;

  need_to_redraw();
}


void
canvas::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

  m_pointing_count = 0;

  m_operation_rect = rectangle(0,0,m_image->get_width(),m_image->get_height());

  m_width  = m_pixel_size*m_image->get_width() ;
  m_height = m_pixel_size*m_image->get_height();
}


void
canvas::
modify_dot(color  new_color, int  x, int  y) noexcept
{
  auto&  pix = m_image->get_pixel(x,y);

    if(pix.color != new_color)
    {
      m_recorder.put(pix.color,x,y);

      pix.color = new_color;


      need_to_redraw();
    }
}




void
canvas::
cancel_drawing() noexcept
{
  m_pointing_count = 0;

    if(m_recorder.get_count())
    {
      m_recorder.reset_count();

      need_to_redraw();
    }
}




void
canvas::
undo() noexcept
{
  m_recorder.rollback(*m_image);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
render(image_cursor  cur) noexcept
{
  const int  w = m_image->get_width();
  const int  h = m_image->get_height();

  m_background_style.render(0,0,m_width,m_height,cur);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = m_image->get_pixel(x,y);

        if(pix.color)
        {
          cur.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
        }
    }}


    if(m_grid)
    {
        for(int  y = 0;  y < h;  ++y)
        {
          cur.draw_hline(predefined_color::light_gray,0,m_pixel_size*y,m_width);
        }


        for(int  x = 0;  x < w;  ++x)
        {
          cur.draw_vline(predefined_color::light_gray,m_pixel_size*x,0,m_height);
        }


      cur.draw_hline(predefined_color::white,0,m_pixel_size*(h/2),m_width);
      cur.draw_vline(predefined_color::white,m_pixel_size*(w/2),0,m_height);
    }
}




}}




