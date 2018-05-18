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


namespace{
void
transfer(const image&  src, rectangle  src_rect, image&  dst, point  dst_pt, bool  layer) noexcept
{
  bool  does_reverse_vertically   = (src_rect.w < 0);
  bool  does_reverse_horizontally = (src_rect.h < 0);

  constexpr int    vertival_reverse_flag = 1;
  constexpr int  horizontal_reverse_flag = 2;

  int  mode = (does_reverse_vertically  ?   vertival_reverse_flag:0)|
              (does_reverse_horizontally? horizontal_reverse_flag:0);

    if(does_reverse_vertically  ){src_rect.w = -src_rect.w;}
    if(does_reverse_horizontally){src_rect.h = -src_rect.h;}


  auto  src_pixptr_base = &src.get_const_pixel(src_rect.x,src_rect.y);
  auto  dst_pixptr_base = &dst.get_pixel(        dst_pt.x,  dst_pt.y);

  int  src_pixptr_add_amount =               1;
  int  src_w                 = src.get_width();

    if(does_reverse_vertically)
    {
      src_pixptr_base += (src_rect.w-1);

      src_pixptr_add_amount = -src_pixptr_add_amount;
    }


    if(does_reverse_horizontally)
    {
      src_pixptr_base += src_w*(src_rect.h-1);

      src_w = -src_w;
    }


  int  dst_w = dst.get_width() ;
  int  dst_h = dst.get_height();

    for(int  y = 0;  y < src_rect.h;  ++y)
    {
      int  dst_y = dst_pt.y+y;

      auto  dst_pixptr = dst_pixptr_base         ;
                         dst_pixptr_base += dst_w;

      auto  src_pixptr = src_pixptr_base         ;
                         src_pixptr_base += src_w;

        if((dst_y >=    0) &&
           (dst_y < dst_h))
        {
            for(int  x = 0;  x < src_rect.w;  ++x)
            {
              int  dst_x = dst_pt.x+x;

                if((dst_x >=    0) &&
                   (dst_x < dst_w))
                {
                  auto  pix = *src_pixptr;

                    if(!layer || pix.color)
                    {
                      *dst_pixptr = pix;
                    }
                }


              dst_pixptr += 1;
              src_pixptr += src_pixptr_add_amount;
            }
        }
    }
}
}


void
paste(const image&  src, rectangle  src_rect, image&  dst, point  dst_pt) noexcept
{
  transfer(src,src_rect,dst,dst_pt,false);
}


void
paste(const image&  src, rectangle  src_rect, image_cursor  dst) noexcept
{
  transfer(src,src_rect,dst.get_image(),dst.get_offset(),false);
}


void
overlay(const image&  src, rectangle  src_rect, image&  dst, point  dst_pt) noexcept
{
  transfer(src,src_rect,dst,dst_pt,true);
}


void
overlay(const image&  src, rectangle  src_rect, image_cursor  dst) noexcept
{
  transfer(src,src_rect,dst.get_image(),dst.get_offset(),true);
}




}}





