#include"libgbstd/image.hpp"
#include"libgbstd/line_maker.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
image::
draw_dot(point  pt, color_index  i) noexcept
{
    if(i)
    {
      m_pixels[(m_width*pt.y)+pt.x].index = i;
    }
}


void
image::
draw_dot_safely(point  pt, color_index  i) noexcept
{
    if((pt.x >=        0) &&
       (pt.x <   m_width) &&
       (pt.y >=        0) &&
       (pt.y <  m_height))
    {
      draw_dot(pt,i);
    }
}


void
image::
draw_dot(point  pt, pixel  pix) noexcept
{
    if(pix.index)
    {
      auto&  dst = m_pixels[(m_width*pt.y)+pt.x];

        if(dst.z <= pix.z)
        {
          dst = pix;
        }
    }
}


void
image::
draw_dot_safely(point  pt, pixel  pix) noexcept
{
    if((pt.x >=        0) &&
       (pt.x <   m_width) &&
       (pt.y >=        0) &&
       (pt.y <  m_height))
    {
      draw_dot(pt,pix);
    }
}


void
image::
draw_vline(point  pt, int  l, pixel  pix) noexcept
{
    while(l--)
    {
      draw_dot(pt,pix);

      pt.y += 1;
    }
}


void
image::
draw_vline_safely(point  pt, int  l, pixel  pix) noexcept
{
    if((pt.x >=       0) &&
       (pt.x <  m_width))
    {
        if(pt.y < 0)
        {
          l += pt.y    ;
               pt.y = 0;
        }

      else
        if(pt.y+l >= m_height)
        {
          l = m_height-pt.y;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(pt,pix);

              pt.y += 1;
            }
        }
    }
}


void
image::
draw_hline(point  pt, int  l, pixel  pix, int  interval) noexcept
{
  int  n = interval;

    while(l--)
    {
        if(n)
        {
          --n;
        }

      else
        {
          draw_dot(pt,pix);

          n = interval;
        }


      pt.x += 1;
    }
}


void
image::
draw_hline_safely(point  pt, int  l, pixel  pix) noexcept
{
    if((pt.y >=        0) &&
       (pt.y <  m_height))
    {
        if(pt.x < 0)
        {
          l += pt.x    ;
               pt.x = 0;
        }

      else
        if(pt.x+l >= m_width)
        {
          l = m_width-pt.x;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(pt,pix);

              pt.x += 1;
            }
        }
    }
}


void
image::
draw_line(line  line, pixel  pix) noexcept
{
  line_maker  lnmk(line);

  draw_dot(lnmk.get_point(),pix);

    while(lnmk.get_distance())
    {
      lnmk.step();

      draw_dot(lnmk.get_point(),pix);
    }
}


void
image::
draw_rectangle(rectangle const&  rect, pixel  pix) noexcept
{
  draw_hline(point(rect.x,rect.y         ),rect.w,pix);
  draw_hline(point(rect.x,rect.y+rect.h-1),rect.w,pix);

  draw_vline(point(rect.x         ,rect.y+1),rect.h-2,pix);
  draw_vline(point(rect.x+rect.w-1,rect.y+1),rect.h-2,pix);
}


void
image::
draw_rectangle_safely(rectangle const&  rect, pixel  pix) noexcept
{
  draw_hline_safely(point(rect.x,rect.y         ),rect.w,pix);
  draw_hline_safely(point(rect.x,rect.y+rect.h-1),rect.w,pix);

  draw_vline_safely(point(rect.x         ,rect.y+1),rect.h-2,pix);
  draw_vline_safely(point(rect.x+rect.w-1,rect.y+1),rect.h-2,pix);
}


void
image::
fill_rectangle(rectangle  rect, pixel  pix) noexcept
{
    while(rect.h--)
    {
      draw_hline(rect,rect.w,pix);

      ++rect.y;
    }
}


}}





