#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
transform(rectangle&  src_rect, rectangle&  dst_rect) noexcept
{
  auto&  src_x = src_rect.x;
  auto&  src_y = src_rect.y;
  auto&  src_w = src_rect.w;
  auto&  src_h = src_rect.h;

  auto&  dst_x = dst_rect.x;
  auto&  dst_y = dst_rect.y;
  auto&  dst_w = dst_rect.w;
  auto&  dst_h = dst_rect.h;

    if(src_w > dst_w)
    {
      src_w = dst_w;
    }


    if(src_h > dst_h)
    {
      src_h = dst_h;
    }


    if(dst_x < 0)
    {
      const int  diff = -dst_x    ;
                         dst_x = 0;

        if(diff < src_w)
        {
          src_x += diff;
          src_w -= diff;
        }

      else
        {
          src_w = 0;

          return;
        }
    }



    if(dst_y < 0)
    {
      const int  diff = -dst_y    ;
                         dst_y = 0;

        if(diff < src_h)
        {
          src_y += diff;
          src_h -= diff;
        }

      else
        {
          src_h = 0;

          return;
        }
    }


    if((dst_x+src_w) >= dst_w)
    {
      const int  diff = (dst_x+src_w)-dst_w;

        if(diff < src_w)
        {
          src_w -= diff;
        }

      else
        {
          src_w = 0;

          return;
        }
    }


    if((dst_y+src_h) >= dst_h)
    {
      const int  diff = (dst_y+src_h)-dst_h;

        if(diff < src_h)
        {
          src_h -= diff;
        }

      else
        {
          src_h = 0;

          return;
        }
    }
}


void
transfer(const image&  src, rectangle  src_rect, image_cursor&  dst, bool  layer) noexcept
{
  auto&  dst_img = dst.get_image();

  rectangle  dst_rect = dst.get_rectangle();

  transform(src_rect,dst_rect);

    if(src_rect.w)
    {
        for(int  y = 0;  y < src_rect.h;  y += 1){
        for(int  x = 0;  x < src_rect.w;  x += 1){
          auto  pix = src.get_const_pixel(src_rect.x+x,src_rect.y+y);

            if(!layer || pix.color)
            {
              dst_img.set_pixel(pix,dst_rect.x+x,dst_rect.y+y);
            }
        }}
    }


  dst.set_rectangle(dst_rect.x,dst_rect.y,src_rect.w,src_rect.h);
}




}}





