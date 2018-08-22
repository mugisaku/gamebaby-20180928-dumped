#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
void
map_color1(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y)
    {
      auto  end = dst+w;

        for(;;)
        {
          auto  v = *src++;

          *dst++ = (v>>7);

            if(dst == end){break;} else{*dst++ = (v>>6)&1;}
            if(dst == end){break;} else{*dst++ = (v>>5)&1;}
            if(dst == end){break;} else{*dst++ = (v>>4)&1;}
            if(dst == end){break;} else{*dst++ = (v>>3)&1;}
            if(dst == end){break;} else{*dst++ = (v>>2)&1;}
            if(dst == end){break;} else{*dst++ = (v>>1)&1;}
            if(dst == end){break;} else{*dst++ = (v   )&1;}
        }
    }
}


void
map_color2(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y)
    {
      auto  end = dst+w;

        for(;;)
        {
          auto  v = *src++;

          *dst++ = (v>>6);

            if(dst == end){break;} else{*dst++ = (v>>4)&0b11;}
            if(dst == end){break;} else{*dst++ = (v>>2)&0b11;}
            if(dst == end){break;} else{*dst++ = (v   )&0b11;}
        }
    }
}


void
map_color4(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y)
    {
      auto  end = dst+w;

        for(;;)
        {
          auto  v = *src++;

          *dst++ = (v>>4);

            if(dst == end){break;} else{*dst++ = v&0b1111;}
        }
    }
}


void
map_color8(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      *dst++ = *src++;
    }}
}


void
copy_rgb(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
    }}
}


void
copy_rgba(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
    }}
}


void
copy_gray1(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
  auto   f = [](uint8_t  v){return v? (((v&1)<<7)|0b1111111):0;};

    for(int  y = 0;  y < h;  ++y)
    {
      auto  end = dst+w;

        for(;;)
        {
          auto  v = *src++;

          *dst++ = f(v>>7);

            if(dst == end){break;} else{*dst++ = f(v>>6);}
            if(dst == end){break;} else{*dst++ = f(v>>5);}
            if(dst == end){break;} else{*dst++ = f(v>>4);}
            if(dst == end){break;} else{*dst++ = f(v>>3);}
            if(dst == end){break;} else{*dst++ = f(v>>2);}
            if(dst == end){break;} else{*dst++ = f(v>>1);}
            if(dst == end){break;} else{*dst++ = f(v   );}
        }
    }
}


void
copy_gray2(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
  auto   f = [](uint8_t  v){return v? (((v&0b11)<<6)|0b111111):0;};

    for(int  y = 0;  y < h;  ++y)
    {
      auto  end = dst+w;

        for(;;)
        {
          auto  v = *src++;

          *dst++ = f(v>>6);

            if(dst == end){break;} else{*dst++ = f(v>>4);}
            if(dst == end){break;} else{*dst++ = f(v>>2);}
            if(dst == end){break;} else{*dst++ = f(v   );}
        }
    }
}


void
copy_gray4(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
  auto   f = [](uint8_t  v){return v? ((v<<4)|0b1111):0;};

    for(int  y = 0;  y < h;  ++y)
    {
      auto  end = dst+w;

        for(;;)
        {
          auto  v = *src++;

          *dst++ = f(v>>4);

            if(dst == end){break;} else{*dst++ = f(v);}
        }
    }
}


void
copy_gray8(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      *dst++ = *src++;
    }}
}


void
copy_gray_with_alpha(const uint8_t*  src, int  w, int  h, uint8_t*  dst) noexcept
{
    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      *dst++ = *src++;
      *dst++ = *src++;
    }}
}
}



binary
image_data::
extract(const image_header&  ihdr) const noexcept
{
  auto  uncompressed = get_uncompressed();
  auto    unfiltered = get_unfiltered(uncompressed.get_data(),ihdr);

  auto  ptr = unfiltered.begin();

  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  bpp = ihdr.get_number_of_bytes_per_pixel();

  int  bit_depth = ihdr.get_bit_depth();

  binary  bin(bpp*w*h);

  uint8_t*  dst = bin.begin();

    switch(ihdr.get_pixel_format())
    {
  case(pixel_format::indexed):
        switch(bit_depth)
        {
      case(1): map_color1(ptr,w,h,dst);break;
      case(2): map_color2(ptr,w,h,dst);break;
      case(4): map_color4(ptr,w,h,dst);break;
      case(8): map_color8(ptr,w,h,dst);break;
        }
      break;
  case(pixel_format::rgba):
      copy_rgba(ptr,w,h,dst);
      break;
  case(pixel_format::rgb):
      copy_rgb(ptr,w,h,dst);
      break;
  case(pixel_format::grayscale_with_alpha):
      copy_gray_with_alpha(ptr,w,h,dst);
      break;
  case(pixel_format::grayscale):
        switch(bit_depth)
        {
      case(1): copy_gray1(ptr,w,h,dst);break;
      case(2): copy_gray2(ptr,w,h,dst);break;
      case(4): copy_gray4(ptr,w,h,dst);break;
      case(8): copy_gray8(ptr,w,h,dst);break;
        }
    }


  return std::move(bin);
}




}




