#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
image::
transfer(image const&  src, rectangle  src_rect, point  dst_pt, int  z) noexcept
{
  bool  reverse_flag = false;

    if(src_rect.w < 0)
    {
      reverse_flag = true;

      src_rect.w = -src_rect.w;
    }


  const int  dst_w = m_width ;
  const int  dst_h = m_height;

    if(!src_rect.w){src_rect.w =  src.get_width();}
    if(!src_rect.h){src_rect.h = src.get_height();}

    if(src_rect.w > dst_w){src_rect.w = dst_w;}
    if(src_rect.h > dst_h){src_rect.h = dst_h;}

    if(dst_pt.x < 0)
    {
      const int  diff = -dst_pt.x    ;
                         dst_pt.x = 0;

        if(diff < src_rect.w)
        {
          src_rect.x += diff;
          src_rect.w -= diff;
        }

      else
        {
          return;
        }
    }



    if(dst_pt.y < 0)
    {
      const int  diff = -dst_pt.y    ;
                         dst_pt.y = 0;

        if(diff < src_rect.h)
        {
          src_rect.y += diff;
          src_rect.h -= diff;
        }

      else
        {
          return;
        }
    }


    if((dst_pt.x+src_rect.w) >= dst_w)
    {
      const int  diff = (dst_pt.x+src_rect.w)-dst_w;

        if(diff < src_rect.w)
        {
          src_rect.w -= diff;
        }

      else
        {
          return;
        }
    }


    if((dst_pt.y+src_rect.h) >= dst_h)
    {
      const int  diff = (dst_pt.y+src_rect.h)-dst_h;

        if(diff < src_rect.h)
        {
          src_rect.h -= diff;
        }

      else
        {
          return;
        }
    }


    for(int  yy = 0;  yy < src_rect.h;  yy += 1)
    {
        if(reverse_flag)
        {
          int  x = src_rect.x+src_rect.w-1;

            for(int  xx = 0;  xx < src_rect.w;  xx += 1)
            {
              auto  pix = src.get_const_pixel(x--,src_rect.y+yy);

              pix.z = z;

              draw_dot(point(dst_pt.x+xx,dst_pt.y+yy),pix);
            }
        }

      else
        {
            for(int  xx = 0;  xx < src_rect.w;  xx += 1)
            {
              auto  pix = src.get_const_pixel(src_rect.x+xx,src_rect.y+yy);

              pix.z = z;

              draw_dot(point(dst_pt.x+xx,dst_pt.y+yy),pix);
            }
        }
    }
}




}}





