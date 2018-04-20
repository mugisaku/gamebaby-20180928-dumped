#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




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
draw_rect(rectangle  rect) noexcept
{
  m_preview_points.resize(0);

    for(int  yy = 0;  yy < rect.h;  ++yy)
    {
      m_preview_points.emplace_back(rect.x         ,rect.y+yy);
      m_preview_points.emplace_back(rect.x+rect.w-1,rect.y+yy);
    }


    for(int  xx = 0;  xx < rect.w;  ++xx)
    {
      m_preview_points.emplace_back(rect.x+xx,rect.y         );
      m_preview_points.emplace_back(rect.x+xx,rect.y+rect.h-1);
    }
}


void
canvas::
fill_rect(rectangle  rect) noexcept
{
  m_preview_points.resize(0);

    for(int  yy = 0;  yy < rect.h;  ++yy){
    for(int  xx = 0;  xx < rect.w;  ++xx){
      m_preview_points.emplace_back(rect.x+xx,rect.y+yy);
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


  m_recorder.push(false);

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

              pix.color = m_drawing_color;

                if(pt.x      ){stack.emplace_back(point(pt.x-1,pt.y  ));}
                if(pt.y      ){stack.emplace_back(point(pt.x  ,pt.y-1));}
                if(pt.x < w-1){stack.emplace_back(point(pt.x+1,pt.y  ));}
                if(pt.y < h-1){stack.emplace_back(point(pt.x  ,pt.y+1));}
            }
        }
    }


  m_recorder.push(true);
}




}}




