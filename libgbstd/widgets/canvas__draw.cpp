#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
canvas::
draw_line(images::color  color, point  a, point  b) noexcept
{
  line_maker  l(a.x,a.y,b.x,b.y);

  m_recorder.push(false);
  m_recorder.reset_count();

    for(;;)
    {
      modify_dot(color,l.get_x(),l.get_y());

        if(!l.get_distance())
        {
          break;
        }


      l.step();
    }


  m_recorder.push(true);

    if(m_recorder.get_count())
    {
      need_to_redraw();
    }
}


void
canvas::
draw_rect(images::color  color, rectangle  rect) noexcept
{
  m_recorder.push(false);
  m_recorder.reset_count();

    for(int  yy = 0;  yy < rect.h;  ++yy)
    {
      modify_dot(color,rect.x         ,rect.y+yy);
      modify_dot(color,rect.x+rect.w-1,rect.y+yy);
    }


    for(int  xx = 0;  xx < rect.w;  ++xx)
    {
      modify_dot(color,rect.x+xx,rect.y         );
      modify_dot(color,rect.x+xx,rect.y+rect.h-1);
    }


  m_recorder.push(true);

  need_to_redraw();
}


void
canvas::
fill_rect(images::color  color, rectangle  rect) noexcept
{
  m_recorder.push(false);
  m_recorder.reset_count();

    for(int  yy = 0;  yy < rect.h;  ++yy){
    for(int  xx = 0;  xx < rect.w;  ++xx){
      modify_dot(color,rect.x+xx,rect.y+yy);
    }}


  m_recorder.push(true);

    if(m_recorder.get_count())
    {
      need_to_redraw();
    }
}


void
canvas::
fill_area(images::color  color, point  pt) noexcept
{
  auto&  img = *m_image;

  auto  target_color = img.get_pixel(pt.x,pt.y).color;

    if(target_color == color)
    {
      return;
    }


  const int  w = m_image->get_width();
  const int  h = m_image->get_height();

    for(auto&  pix: img)
    {
      pix.z = 0;
    }


  m_recorder.push(false);

  m_recorder.reset_count();


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
              m_recorder.put(pix.color,pt.x,pt.y);

              pix.color = color;

                if(pt.x      ){stack.emplace_back(point(pt.x-1,pt.y  ));}
                if(pt.y      ){stack.emplace_back(point(pt.x  ,pt.y-1));}
                if(pt.x < w-1){stack.emplace_back(point(pt.x+1,pt.y  ));}
                if(pt.y < h-1){stack.emplace_back(point(pt.x  ,pt.y+1));}
            }
        }
    }


  m_recorder.push(true);

    if(m_recorder.get_count())
    {
      m_recorder.reset_count();

      need_to_redraw();
    }
}




}}




