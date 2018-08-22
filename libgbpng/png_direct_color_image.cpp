#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




direct_color_image&
direct_color_image::
assign(image_source&  isrc) noexcept
{
  int  w = isrc.ihdr.get_width() ;
  int  h = isrc.ihdr.get_height();

  auto&  plte = isrc.plte;
  auto&  trns = isrc.trns;
  auto&  bkgd = isrc.bkgd;

  auto      src_p = isrc.data.begin();
  uint8_t*  dst_p;

    switch(isrc.ihdr.get_pixel_format())
    {
  case(pixel_format::indexed):
      dst_p = allocate(w,h);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto  i = *src_p++;

          auto  a = trns.get_alpha(i);

            if(a)
            {
              auto&  color = plte.get_color(i);

              *dst_p++ = color.r;
              *dst_p++ = color.g;
              *dst_p++ = color.b;
              *dst_p++ =       a;
            }

          else
            {
              *dst_p++ = 0;
              *dst_p++ = 0;
              *dst_p++ = 0;
              *dst_p++ = 0;
            }
        }}
      break;
  case(pixel_format::grayscale):
      dst_p = allocate(w,h);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto  i = *src_p++;

          auto  a = trns.get_alpha(static_cast<uint16_t>(i));

            if(a)
            {
              *dst_p++ = i;
              *dst_p++ = i;
              *dst_p++ = i;
              *dst_p++ = a;
            }

          else
            {
              *dst_p++ = 0;
              *dst_p++ = 0;
              *dst_p++ = 0;
              *dst_p++ = 0;
            }
        }}
      break;
  case(pixel_format::grayscale_with_alpha):
      dst_p = allocate(w,h);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          *dst_p++ = *src_p;
          *dst_p++ = *src_p;
          *dst_p++ = *src_p++;
          *dst_p++ = *src_p++;
        }}
      break;
  case(pixel_format::rgb):
      dst_p = allocate(w,h);

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto  r = *src_p++;
          auto  g = *src_p++;
          auto  b = *src_p++;

          auto  a = trns.get_alpha(r,g,b);

            if(a)
            {
              *dst_p++ = r;
              *dst_p++ = g;
              *dst_p++ = b;
              *dst_p++ = a;
            }

          else
            {
              *dst_p++ = 0;
              *dst_p++ = 0;
              *dst_p++ = 0;
              *dst_p++ = 0;
            }
        }}
      break;
  case(pixel_format::rgba):
      store(std::move(isrc.data),w,h);
      break;
    }


  return *this;
}


void
direct_color_image::
save_file(const char*  path, pixel_format  fmt, int  bit_depth) const noexcept
{
  chunk_list  ls;

  image_header  ihdr(get_width(),get_height(),fmt);

  ihdr.set_bit_depth(bit_depth);

  ls.push_back(ihdr.make_chunk());


  auto  idat = get_image_data(fmt,bit_depth);

  ls.push_back(idat.make_chunk());

  ls.write_png_to_file(path);
}


void
direct_color_image::
load_file(const char*  path) noexcept
{
  chunk_list  ls;

  ls.read_png_from_file(path);


  chunk_set  set(ls);

  image_source  isrc;

  isrc.ihdr = set.get_image_header();

  auto  idat = set.get_image_data();

  isrc.data = idat.extract(isrc.ihdr);

    if(isrc.ihdr.get_bit_depth() < 8)
    {
      isrc.data = get_unbitpacked(isrc.data.begin(),isrc.ihdr);
    }


  auto  trns_chunk = set.get_trns_chunk();
  auto  plte_chunk = set.get_plte_chunk();

    if(trns_chunk)
    {
      isrc.trns.assign(*trns_chunk,isrc.ihdr.get_pixel_format());
    }


    if(plte_chunk)
    {
      isrc.plte.assign(*plte_chunk);
    }


  assign(isrc);
}




image_data
direct_color_image::
get_image_data(pixel_format  fmt, int  bit_depth) const noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  auto  src = get_row_pointer(0);

  image_header  ihdr(w,h,fmt);

  int  bpp = ihdr.get_number_of_bytes_per_pixel();

  ihdr.set_bit_depth(bit_depth);

    if(fmt == pixel_format::rgba)
    {
      return image_data(src,ihdr);
    }


  binary  bin(bpp*w*h);

  uint8_t*  dst = bin.begin();

    switch(fmt)
    {
  case(pixel_format::indexed):
      break;
  case(pixel_format::grayscale):
      for(int  y = 0;  y < h;  ++y){
      for(int  x = 0;  x < w;  ++x){
        uint32_t  v  = *src++;
                  v += *src++;
                  v += *src++;
                        ++src;

        *dst++ = v/3;
      }}
      break;
  case(pixel_format::grayscale_with_alpha):
      for(int  y = 0;  y < h;  ++y){
      for(int  x = 0;  x < w;  ++x){
        uint32_t  v  = *src++;
                  v += *src++;
                  v += *src++;

        *dst++ = v/3;
        *dst++ = *src++;
      }}
      break;
  case(pixel_format::rgb):
      for(int  y = 0;  y < h;  ++y){
      for(int  x = 0;  x < w;  ++x){
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
                  ++src;
      }}
      break;
    }


    if((fmt == pixel_format::grayscale) && (bit_depth < 8))
    {
      bin = get_bitpacked(bin.begin(),ihdr);
    }


  return image_data(bin.begin(),ihdr);
}




}




