#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
void
pack(const uint8_t*  src, int  w, int  bit_depth, uint8_t*  dst) noexcept
{
    if(bit_depth == 8)
    {
        for(int  x = 0;  x < w;  ++x)
        {
          *dst++ = *src++;
        }
    }

  else
    if(bit_depth == 4)
    {
        for(int  x = 0;  x < w;  ++x)
        {
          *dst    = (*src++)<<4;
          *dst++ |= (*src++)&0b1111;
        }
    }

  else
    if(bit_depth == 2)
    {
        for(int  x = 0;  x < w;  ++x)
        {
          *dst    = ((*src++)&0b11)<<6;
          *dst   |= ((*src++)&0b11)<<4;
          *dst   |= ((*src++)&0b11)<<2;
          *dst++ |= ((*src++)&0b11)   ;
        }
    }

  else
    if(bit_depth == 1)
    {
        for(int  x = 0;  x < w;  ++x)
        {
          *dst    = ((*src++)&0b1)<<7;
          *dst   |= ((*src++)&0b1)<<6;
          *dst   |= ((*src++)&0b1)<<5;
          *dst   |= ((*src++)&0b1)<<4;
          *dst   |= ((*src++)&0b1)<<3;
          *dst   |= ((*src++)&0b1)<<2;
          *dst   |= ((*src++)&0b1)<<1;
          *dst++ |= ((*src++)&0b1)   ;
        }
    }

  else
    {
      printf("get_filtered error: invalid bit_depth\n");
    }
}
}




binary
get_filtered(const uint8_t*  src, const image_header&  ihdr) noexcept
{
  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  pitch = ihdr.get_pitch();

  int  bit_depth = ihdr.get_bit_depth();

  int  bpp = get_number_of_bytes_per_pixel(ihdr.get_pixel_format());

  binary  tmp((pitch+1)*h);

  auto  dst = tmp.begin();

    for(int  y = 0;  y < h;  ++y)
    {
      *dst++ = 0;

      pack(src,w,bit_depth,dst);

      src +=     w;
      dst += pitch;
    }


  return std::move(tmp);
}




}




