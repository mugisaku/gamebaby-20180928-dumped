#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




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

  m_operation_rect = rectangle(0,0,img.get_width(),img.get_height());

  need_to_reform();
}


void
canvas::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

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
    }
}




void
canvas::
apply() noexcept
{
  m_pointing_count = 0;

    if(m_preview_points.size())
    {
      m_recorder.push(false);

        for(auto&  pt: m_preview_points)
        {
          modify_dot(m_drawing_color,pt.x,pt.y);
        }


      m_preview_points.resize(0);

      m_recorder.push(true);

      need_to_redraw();

        if(m_callback)
        {
          m_callback(*this);
        }
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
do_when_mouse_acted(int  x, int  y) noexcept
{
  x /= m_pixel_size;
  y /= m_pixel_size;

    switch(m_mode)
    {
  case(mode::draw_dot):
        if(ctrl.is_mouse_lbutton_pressed())
        {
          modify_dot(m_drawing_color,x,y);
          need_to_redraw();

            if(m_callback)
            {
              m_callback(*this);
            }
        }

      else
        if(ctrl.is_mouse_rbutton_pressed())
        {
          modify_dot(color(),x,y);
          need_to_redraw();

            if(m_callback)
            {
              m_callback(*this);
            }
        }
      break;
  case(mode::draw_line):
        if(m_pointing_count)
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              draw_line(m_a_point,point(x,y));

              need_to_redraw();
            }

          else
            {
              apply();
            }
        }

      else
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::draw_rectangle):
        if(m_pointing_count)
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              draw_rect(make_rectangle(m_a_point,point(x,y)));

              need_to_redraw();
            }

          else
            {
              apply();
            }
        }

      else
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_rectangle):
        if(m_pointing_count)
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              fill_rect(make_rectangle(m_a_point,point(x,y)));

              need_to_redraw();
            }

          else
            {
              apply();
            }
        }

      else
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_area):
        if(ctrl.is_mouse_lbutton_pressed())
        {
          fill_area(point(x,y));

          need_to_redraw();

            if(m_callback)
            {
              m_callback(*this);
            }
        }
      break;
    }
}


void
canvas::
render(image_cursor  cur) noexcept
{
  auto&  img = *m_image;

  const int  w = m_image->get_width();
  const int  h = m_image->get_height();

  int           pitch = m_pixel_size*w;
  int  number_of_rows = m_pixel_size*h;

    for(int  y = 0;  y < number_of_rows;  y += 2)
    {
      cur.draw_hline(color(01005),0,y  ,pitch);
      cur.draw_hline(color(01337),0,y+1,pitch);
    }


    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = img.get_pixel(x,y);

        if(pix.color)
        {
          cur.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
        }
    }}


    for(auto&  pt: m_preview_points)
    {
      cur.fill_rectangle(m_drawing_color,m_pixel_size*pt.x,m_pixel_size*pt.y,m_pixel_size,m_pixel_size);
    }


    if(m_grid)
    {
        for(int  y = 0;  y < h;  ++y)
        {
          cur.draw_hline(predefined_color::gray,0,m_pixel_size*y,pitch);
        }


        for(int  x = 0;  x < w;  ++x)
        {
          cur.draw_vline(predefined_color::gray,m_pixel_size*x,0,number_of_rows);
        }


      cur.draw_hline(predefined_color::light_gray,0,m_pixel_size*(h/2),pitch);
      cur.draw_vline(predefined_color::light_gray,m_pixel_size*(w/2),0,number_of_rows);
    }
}




}}




