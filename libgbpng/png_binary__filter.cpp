#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<zlib.h>




namespace gbpng{




binary
binary::
get_filtered(const image_header&  ihdr) const noexcept
{
  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  binary  tmp(((4*w)+1)*h);

  auto  dst = tmp.begin();
  auto  src =     begin();

    for(int  y = 0;  y < h;  ++y)
    {
      *dst++ = 0;

        for(int  x = 0;  x < w;  ++x)
        {
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = *src++;
        }
    }


  return std::move(tmp);
}




}




