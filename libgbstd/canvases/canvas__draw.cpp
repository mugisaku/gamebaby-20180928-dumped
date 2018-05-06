#include"libgbstd/canvas.hpp"



namespace gbstd{
namespace canvases{




void
canvas::
draw_line(colors::color  color, point  a, point  b) noexcept
{
  line_maker  l(a.x,a.y,b.x,b.y);

  try_to_push_nonsolid_record();

    for(;;)
    {
      modify_dot(color,l.get_x(),l.get_y());

        if(!l.get_distance())
        {
          break;
        }


      l.step();
    }


  try_to_push_solid_record();
}


void
canvas::
draw_rect(colors::color  color, rectangle  rect) noexcept
{
  try_to_push_nonsolid_record();

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


  try_to_push_solid_record();
}


void
canvas::
fill_rect(colors::color  color, rectangle  rect) noexcept
{
  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < rect.h;  ++yy){
    for(int  xx = 0;  xx < rect.w;  ++xx){
      modify_dot(color,rect.x+xx,rect.y+yy);
    }}


  try_to_push_solid_record();
}


void
canvas::
fill_area(colors::color  color, point  pt) noexcept
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


  try_to_push_nonsolid_record();


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


  try_to_push_solid_record();

  m_drawing_is_fixed = true;
}




}}




