#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




void
direct_color_image::
load_file(const char*  path) noexcept
{
  chunk_list  ls;

  ls.read_png_from_file(path);


  chunk_set  set(ls);

  auto  ihdr = set.get_image_header();
  auto  idat = set.get_image_data();

  auto  bin = idat.extract(ihdr);

  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  const uint8_t*  src = bin.get_data();
        uint8_t*  dst;

  palette  plte;

  transparency_info  trns;

  auto  trns_chunk = set.get_trns_chunk();

    if(trns_chunk)
    {
      trns.assign(*trns_chunk,ihdr.get_pixel_format());
    }


    switch(ihdr.get_pixel_format())
    {
  case(pixel_format::indexed):
        if(set.get_plte_chunk())
        {
          plte = palette(*set.get_plte_chunk());

          allocate(w,h);

          dst = get_row_pointer(0);

            for(int  y = 0;  y < h;  ++y){
            for(int  x = 0;  x < w;  ++x){
              auto  i = *src++;

              auto  a = trns.get_alpha(i);

                if(a)
                {
                  auto&  color = plte.get_color(i);

                  *dst++ = color.r;
                  *dst++ = color.g;
                  *dst++ = color.b;
                  *dst++ =       a;
                }

              else
                {
                  *dst++ = 0;
                  *dst++ = 0;
                  *dst++ = 0;
                  *dst++ = 0;
                }
            }}
        }
      break;
  case(pixel_format::grayscale):
      allocate(w,h);

      dst = get_row_pointer(0);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto  i = *src++;

          auto  a = trns.get_alpha(static_cast<uint16_t>(i));

            if(a)
            {
              *dst++ = i;
              *dst++ = i;
              *dst++ = i;
              *dst++ = a;
            }

          else
            {
              *dst++ = 0;
              *dst++ = 0;
              *dst++ = 0;
              *dst++ = 0;
            }
        }}
      break;
  case(pixel_format::grayscale_with_alpha):
      allocate(w,h);

      dst = get_row_pointer(0);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          *dst++ = *src;
          *dst++ = *src;
          *dst++ = *src++;
          *dst++ = *src++;
        }}
      break;
  case(pixel_format::rgb):
      allocate(w,h);

      dst = get_row_pointer(0);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto  r = *src++;
          auto  g = *src++;
          auto  b = *src++;

          auto  a = trns.get_alpha(r,g,b);

            if(a)
            {
              *dst++ = r;
              *dst++ = g;
              *dst++ = b;
              *dst++ = a;
            }

          else
            {
              *dst++ = 0;
              *dst++ = 0;
              *dst++ = 0;
              *dst++ = 0;
            }
        }}
      break;
  case(pixel_format::rgba):
      store(std::move(bin),w,h);
      break;
    }


  printf("%s{\n",path);

  ihdr.print();

  printf("}\n\n");
}




}




