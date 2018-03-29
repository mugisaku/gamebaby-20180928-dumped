#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
transfer(const image_frame&  src, const image_cursor&  dst) noexcept
{
  bool  reverse_flag = false;

  int  src_x = src.get_x_offset();
  int  src_y = src.get_y_offset();
  int  src_w = src.get_width();
  int  src_h = src.get_height();

  int  dst_x = dst.get_x_offset();
  int  dst_y = dst.get_y_offset();
  int  dst_w = dst.get_image().get_width();
  int  dst_h = dst.get_image().get_height();

    if(src_w < 0)
    {
      reverse_flag = true;

      src_w = -src_w;
    }


    if(src_w > dst_w){src_w = dst_w;}
    if(src_h > dst_h){src_h = dst_h;}

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
          return;
        }
    }


  auto&  src_img = src.get_image();
  auto&  dst_img = dst.get_image();

    for(int  yy = 0;  yy < src_h;  yy += 1)
    {
        if(reverse_flag)
        {
          int  x = src_x+src_w-1;

            for(int  xx = 0;  xx < src_w;  xx += 1)
            {
              auto  pix = src_img.get_const_pixel(x--,src_y+yy);

              dst_img.draw_dot(pix,dst_x+xx,dst_y+yy);
            }
        }

      else
        {
            for(int  xx = 0;  xx < src_w;  xx += 1)
            {
              auto  pix = src_img.get_const_pixel(src_x+xx,src_y+yy);

              dst_img.draw_dot(pix,dst_x+xx,dst_y+yy);
            }
        }
    }
}




void
transfer(const image_frame&  src, const image_cursor&  dst, int  z) noexcept
{
  bool  reverse_flag = false;

  int  src_x = src.get_x_offset();
  int  src_y = src.get_y_offset();
  int  src_w = src.get_width();
  int  src_h = src.get_height();

  int  dst_x = dst.get_x_offset();
  int  dst_y = dst.get_y_offset();
  int  dst_w = dst.get_image().get_width();
  int  dst_h = dst.get_image().get_height();

    if(src_w < 0)
    {
      reverse_flag = true;

      src_w = -src_w;
    }


    if(src_w > dst_w){src_w = dst_w;}
    if(src_h > dst_h){src_h = dst_h;}

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
          return;
        }
    }


  auto&  src_img = src.get_image();
  auto&  dst_img = dst.get_image();

    for(int  yy = 0;  yy < src_h;  yy += 1)
    {
        if(reverse_flag)
        {
          int  x = src_x+src_w-1;

            for(int  xx = 0;  xx < src_w;  xx += 1)
            {
              auto  pix = src_img.get_const_pixel(x--,src_y+yy);

              pix.z = z;

              dst_img.draw_dot(pix,dst_x+xx,dst_y+yy);
            }
        }

      else
        {
            for(int  xx = 0;  xx < src_w;  xx += 1)
            {
              auto  pix = src_img.get_const_pixel(src_x+xx,src_y+yy);

              pix.z = z;

              dst_img.draw_dot(pix,dst_x+xx,dst_y+yy);
            }
        }
    }
}




}}





