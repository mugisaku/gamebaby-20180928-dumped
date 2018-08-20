#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
uint8_t
bitpack_grayscale4(const uint8_t*  ptr) noexcept
{
  return((ptr[0]>>4)<<4)|
        ((ptr[1]>>4)   );
}
uint8_t
bitpack_grayscale2(const uint8_t*  ptr) noexcept
{
  return((ptr[0]>>6)<<6)|
        ((ptr[1]>>6)<<4)|
        ((ptr[2]>>6)<<2)|
        ((ptr[3]>>6)   );
}
uint8_t
bitpack_grayscale1(const uint8_t*  ptr) noexcept
{
  return((ptr[0]>>7)<<7)|
        ((ptr[1]>>7)<<6)|
        ((ptr[2]>>7)<<5)|
        ((ptr[3]>>7)<<4)|
        ((ptr[4]>>7)<<3)|
        ((ptr[5]>>7)<<2)|
        ((ptr[6]>>7)<<1)|
        ((ptr[7]>>7)   );
}
}


binary
get_filtered(const uint8_t*  src, const image_header&  ihdr) noexcept
{
  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  bit_depth = ihdr.get_bit_depth();

  auto  fmt = ihdr.get_pixel_format();

  int  src_pitch = ihdr.get_pitch();
  int  dst_pitch = (bit_depth < 8)? ((w+(8-bit_depth))/8*8):src_pitch;

  binary  tmp((dst_pitch+1)*h);

  auto  dst = tmp.begin();

    for(int  y = 0;  y < h;  ++y)
    {
      *dst++ = 0;

        if(bit_depth == 8)
        {
            for(int  x = 0;  x < src_pitch;  ++x)
            {
              *dst++ = *src++;
            }
        }

      else
        {
            if(fmt == pixel_format::indexed)
            {
                switch(bit_depth)
                {
              case(4):
                    for(int  x = 0;  x < (w/2);  ++x)
                    {
                      *dst    = (*src++)<<4;
                      *dst++ |= (*src++)&0b1111;
                    }
                  break;
              case(2):
                    for(int  x = 0;  x < (w/4);  ++x)
                    {
                      *dst    = ((*src++)&0b11)<<6;
                      *dst   |= ((*src++)&0b11)<<4;
                      *dst   |= ((*src++)&0b11)<<2;
                      *dst++ |= ((*src++)&0b11)   ;
                    }
                  break;
              case(1):
                    for(int  x = 0;  x < (w/8);  ++x)
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
                  break;
                }
            }

          else
            if(fmt == pixel_format::grayscale)
            {
              int  n = w;

                switch(bit_depth)
                {
              case(4):
                    while(n)
                    {
                      uint8_t  table[2] = {0};

                        for(int  i = 0;  i < 2;  ++i)
                        {
                          table[i] = *src++;

                            if(!--n)
                            {
                              break;
                            }
                        }


                      *dst++ = bitpack_grayscale4(table);
                    }
                  break;
              case(2):
                    while(n)
                    {
                      uint8_t  table[4] = {0};

                        for(int  i = 0;  i < 4;  ++i)
                        {
                          table[i] = *src++;

                            if(!--n)
                            {
                              break;
                            }
                        }


                      *dst++ = bitpack_grayscale4(table);
                    }
                  break;
              case(1):
                    while(n)
                    {
                      uint8_t  table[8] = {0};

                        for(int  i = 0;  i < 8;  ++i)
                        {
                          table[i] = *src++;

                            if(!--n)
                            {
                              break;
                            }
                        }


                      *dst++ = bitpack_grayscale1(table);
                    }
                  break;
                }
            }
        }
    }


  return std::move(tmp);
}




}




