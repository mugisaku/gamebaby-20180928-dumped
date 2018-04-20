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
      m_recorder.push(pix.color,x,y);

      pix.color = new_color;
    }
}




void
canvas::
draw_line(point  a, point  b) noexcept
{
  line_maker  l(a.x,a.y,b.x,b.y);

  m_preview_points.resize(0);

    for(;;)
    {
      m_preview_points.emplace_back(l.get_x(),l.get_y());

        if(!l.get_distance())
        {
          break;
        }


      l.step();
    }
}


void
canvas::
draw_rect(point  a, point  b) noexcept
{
  int  x = std::min(a.x,b.x);
  int  y = std::min(a.y,b.y);
  int  w = std::abs(a.x-b.x);
  int  h = std::abs(a.y-b.y);

  m_preview_points.resize(0);

    for(int  yy = 0;  yy <= h;  ++yy)
    {
      m_preview_points.emplace_back(x  ,y+yy);
      m_preview_points.emplace_back(x+w,y+yy);
    }


    for(int  xx = 0;  xx <= w;  ++xx)
    {
      m_preview_points.emplace_back(x+xx,y  );
      m_preview_points.emplace_back(x+xx,y+h);
    }
}


void
canvas::
fill_rect(point  a, point  b) noexcept
{
  int  x = std::min(a.x,b.x);
  int  y = std::min(a.y,b.y);
  int  w = std::abs(a.x-b.x);
  int  h = std::abs(a.y-b.y);

  m_preview_points.resize(0);

    for(int  yy = 0;  yy <= h;  ++yy){
    for(int  xx = 0;  xx <= w;  ++xx){
      m_preview_points.emplace_back(x+xx,y+yy);
    }}
}


void
canvas::
fill_area(point  pt) noexcept
{
  auto&  img = *m_image;

  auto  target_color = img.get_pixel(pt.x,pt.y).color;

    if(target_color == m_drawing_color)
    {
      return;
    }


  const int  w = m_image->get_width();
  const int  h = m_image->get_height();

    for(auto&  pix: img)
    {
      pix.z = 0;
    }


  m_recorder.commit(false);

  std::vector<point>  stack;

  int  i = 0;

  stack.emplace_back(pt);

    while(i < stack.size())
    {
      auto  pt = stack[i++];

      auto&  pix = img.get_pixel(pt.x,pt.y);

        if(!pix.z)
        {
          pix.z = 1;

            if(pix.color == target_color)
            {
              m_recorder.push(pix.color,pt.x,pt.y);

              pix.color = m_drawing_color;

                if(pt.x      ){stack.emplace_back(point(pt.x-1,pt.y  ));}
                if(pt.y      ){stack.emplace_back(point(pt.x  ,pt.y-1));}
                if(pt.x < w-1){stack.emplace_back(point(pt.x+1,pt.y  ));}
                if(pt.y < h-1){stack.emplace_back(point(pt.x  ,pt.y+1));}
            }
        }
    }


  m_recorder.commit(true);
}




void
canvas::
apply() noexcept
{
  m_pointing_count = 0;

    if(m_preview_points.size())
    {
      m_recorder.commit(false);

        for(auto&  pt: m_preview_points)
        {
          modify_dot(m_drawing_color,pt.x,pt.y);
        }


      m_preview_points.resize(0);

      m_recorder.commit(true);

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
              draw_rect(m_a_point,point(x,y));

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
              fill_rect(m_a_point,point(x,y));

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




