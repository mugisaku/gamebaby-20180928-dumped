#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{


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
reset(image&  img, int  pixel_size) noexcept
{
  m_image      =       &img;
  m_pixel_size = pixel_size;

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
      m_recorder.push(new_color,x,y);

      pix.color = new_color;
    }
}




void
canvas::
draw_line(images::color  color, point  a, point  b) noexcept
{
  m_recorder.commit(false);

  line_maker  l(a.x,a.y,b.x,b.y);

    for(;;)
    {
      modify_dot(color,l.get_x(),l.get_y());

        if(!l.get_distance())
        {
          break;
        }


      l.step();
    }


  m_recorder.commit(true);
}


void
canvas::
draw_rect(images::color  color, point  a, point  b) noexcept
{
  m_recorder.commit(false);

  int  x = std::min(a.x,b.x);
  int  y = std::min(a.y,b.y);
  int  w = std::abs(a.x-b.x);
  int  h = std::abs(a.y-b.y);

    for(int  yy = 0;  yy <= h;  ++yy)
    {
      modify_dot(color,x  ,y+yy);
      modify_dot(color,x+w,y+yy);
    }


    for(int  xx = 0;  xx <= w;  ++xx)
    {
      modify_dot(color,x   ,y+h);
      modify_dot(color,x+xx,y+h);
    }


  m_recorder.commit(true);
}


void
canvas::
fill_rect(images::color  color, point  a, point  b) noexcept
{
  m_recorder.commit(false);

  int  x = std::min(a.x,b.x);
  int  y = std::min(a.y,b.y);
  int  w = std::abs(a.x-b.x);
  int  h = std::abs(a.y-b.y);

    for(int  yy = 0;  yy <= h;  ++yy){
    for(int  xx = 0;  xx <= w;  ++xx){
      modify_dot(color,x+xx,y+yy);
    }}


  m_recorder.commit(true);
}


void
canvas::
fill_area(images::color  color, point  pt) noexcept
{
  m_recorder.commit(false);

  auto&  img = *m_image;

  const int  w = m_image->get_width();
  const int  h = m_image->get_height();

    for(auto&  pix: img)
    {
      pix.z = 0;
    }


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

            if(pix.color != color)
            {
              m_recorder.push(color,pt.x,pt.y);

              pix.color = color;

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
undo() noexcept
{
  m_recorder.rollback(*m_image);

  need_to_redraw();
}


void
canvas::
render(image_cursor  cur) noexcept
{
  auto&  img = *m_image;

  const int  w = m_image->get_width();
  const int  h = m_image->get_height();

  cur.fill_rectangle(predefined_color::blue,0,0,m_pixel_size*w,m_pixel_size*h);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = img.get_pixel(x,y);

        if(pix.color)
        {
          cur.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
        }
    }}


    if(m_grid)
    {
        for(int  y = 0;  y < h;  ++y)
        {
          cur.draw_hline(predefined_color::gray,0,m_pixel_size*y,m_pixel_size*w);
        }


        for(int  x = 0;  x < w;  ++x)
        {
          cur.draw_vline(predefined_color::gray,m_pixel_size*x,0,m_pixel_size*h);
        }


      cur.draw_hline(predefined_color::light_gray,0,m_pixel_size*(h/2),m_pixel_size*w);
      cur.draw_vline(predefined_color::light_gray,m_pixel_size*(w/2),0,m_pixel_size*h);
    }
}




}}




