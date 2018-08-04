#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
void
unfilter_none(const uint8_t*  src, int  bpp, int  w, uint8_t*  dst) noexcept
{
    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          *dst++ = *src++;
        }
    }
}


void
unfilter_sub(const uint8_t*  src, int  bpp, int  w, uint8_t*  dst) noexcept
{
  const uint8_t  dummy[bpp] = {0};

  const uint8_t*  left_src = dummy;

    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto   cur =      *src++;
          auto  left = *left_src++;

          *dst++ = cur+left;
        }


      left_src = dst-bpp;
    }
}


void
unfilter_up(const uint8_t*  src, const uint8_t*  up_src, int  bpp, int  w, uint8_t*  dst) noexcept
{
    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto   cur =    *src++;
          auto    up = *up_src++;

          *dst++ = (cur+up);
        }
    }
}


void
unfilter_average(const uint8_t*  src, const uint8_t*  up_src, int  bpp, int  w, uint8_t*  dst) noexcept
{
  const uint8_t  dummy[bpp] = {0};

  const uint8_t*  left_src = dummy;

    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto   cur =      *src++;
          auto    up =   *up_src++;
          auto  left = *left_src++;

          *dst++ = cur-((left+up)/2);
        }


      left_src = dst-bpp;
    }
}


constexpr int
paeth(int  a, int  b, int  c) noexcept
{
  int  p = a+b-c;

  int  pa = std::abs(p-a);
  int  pb = std::abs(p-b);
  int  pc = std::abs(p-c);

    if((pa <= pb) && (pa <= pc))
    {
      return a;
    }


    if(pb <= pc)
    {
      return b;
    }


  return c;
}


void
unfilter_paeth(const uint8_t*  src, const uint8_t*  up_src, int  bpp, int  w, uint8_t*  dst) noexcept
{
  const uint8_t  dummy[bpp] = {0};

  const uint8_t*     left_src = dummy;
  const uint8_t*  up_left_src = dummy;

    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto      cur =         *src++;
          auto       up =      *up_src++;
          auto     left =    *left_src++;
          auto  up_left = *up_left_src++;

          *dst++ = cur+paeth(left,up,up_left);
        }


         left_src =    dst-bpp;
      up_left_src = up_src-bpp;
    }
}
}


void
file::
unfilter(const uint8_t*   src, const image_header&  ihdr,
               uint8_t*&  dst) noexcept
{
  constexpr int     none = 0;
  constexpr int      sub = 1;
  constexpr int       up = 2;
  constexpr int  average = 3;
  constexpr int    paeth = 4;

  int  bpp = ihdr.get_number_of_bytes_per_pixel();

  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  pitch = bpp*w;


  auto  ptr = new uint8_t[pitch*h];

  std::memset(ptr,0,pitch*h);

  dst = ptr;


  const uint8_t  dummy_line[pitch] = {0};

  const uint8_t*  up_src = dummy_line;

  int     none_count = 0;
  int      sub_count = 0;
  int       up_count = 0;
  int  average_count = 0;
  int    paeth_count = 0;

    for(int  y = 0;  y < h;  ++y)
    {
      auto  filter_type = *src++;

        if(filter_type == none)
        {
          ++none_count;

          unfilter_none(src,bpp,w,dst);
        }

      else
        if(filter_type == sub)
        {
          ++sub_count;

          unfilter_sub(src,bpp,w,dst);
        }

      else
        if(filter_type == up)
        {
          ++up_count;

          unfilter_up(src,up_src,bpp,w,dst);
        }

      else
        if(filter_type == average)
        {
          ++average_count;

          unfilter_average(src,up_src,bpp,w,dst);
        }

      else
        if(filter_type == paeth)
        {
          ++paeth_count;

          unfilter_paeth(src,up_src,bpp,w,dst);
        }

      else
        {
          printf("png file unfilter error: unkmown filter type\n");

          break;
        }


      up_src = dst;

      src += pitch;
      dst += pitch;
    }


  dst = ptr;

  printf("   none: %6d\n",   none_count);
  printf("    sub: %6d\n",    sub_count);
  printf("     up: %6d\n",     up_count);
  printf("average: %6d\n",average_count);
  printf("  paeth: %6d\n",  paeth_count);
}




}




