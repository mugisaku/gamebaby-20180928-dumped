#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<zlib.h>




namespace gbpng{




image::
image(const image_header&  ihdr) noexcept
{
  resize(ihdr.get_width(),ihdr.get_height());
}




image&
image::
operator=(const image&  rhs) noexcept
{
    if(this != &rhs)
    {
      assign(rhs.get_width(),rhs.get_height(),rhs.m_data);
    }


  return *this;
}


image&
image::
operator=(image&&  rhs) noexcept
{
    if(this != &rhs)
    {
      delete[] m_data          ;
               m_data = nullptr;

      std::swap(m_width ,rhs.m_width );
      std::swap(m_height,rhs.m_height);
      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




image&
image::
assign(int  w, int  h, uint8_t*  data) noexcept
{
    if(data)
    {
      delete[] m_data       ;
               m_data = data;


      m_width  = w;
      m_height = h;
    }

  else
    {
      resize(w,h);
    }


  return *this;
}


image&
image::
assign(int  w, int  h, const uint8_t*  src_data) noexcept
{
  resize(w,h);

    if(src_data)
    {
      std::memcpy(m_data,src_data,4*w*h);
    }


  return *this;
}


image&
image::
assign(const image_header&  ihdr, const palette*  plte, const image_data&  idat) noexcept
{
  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  resize(w,h);

  auto  uncompressed = idat.get_uncompressed();
  auto    unfiltered = uncompressed.get_unfiltered(ihdr);

  auto  ptr = unfiltered.begin();

    if(ihdr.does_use_palette())
    {
        if(!plte)
        {
          printf("png make image error: not found palette\n");
        }

      else
        {
          map_color(ptr,*plte);
        }
    }

  else
    {
        if(ihdr.does_use_color())
        {
            if(ihdr.does_use_alpha())
            {
              copy_rgba(ptr);
            }

          else
            {
              copy_rgb(ptr);
            }
        }

      else
        {
            if(ihdr.does_use_alpha())
            {
              copy_gray_with_alpha(ptr);
            }

          else
            {
              copy_gray(ptr);
            }
        }
    }


  return *this;
}


image&
image::
assign(const image_header&  ihdr, const palette*  plte, const frame_control&  fctl, const frame_data&  fdat) noexcept
{
  auto  tmp_ihdr = ihdr+fctl;

  return assign(tmp_ihdr,plte,fdat);
}


image&
image::
assign(const chunk_list&  ls) noexcept
{
  chunk_set  set(ls);

  image_header  ihdr = set.get_image_header();
  image_data    idat = set.get_image_data();

  return assign(ihdr,nullptr,idat);
}




void
image::
fill() noexcept
{
  std::memset(m_data,0,4*m_width*m_height);
}


void
image::
resize(int  w, int  h) noexcept
{
  const size_t  size = 4*w*h;

  delete[] m_data                    ;
           m_data = new uint8_t[size];

  m_width  = w;
  m_height = h;

  fill();
}




void
image::
map_color(const uint8_t*  src, const palette&  plte) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  auto  dst = get_rgba_pointer(0,0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  color = plte.get_color(*src++);

      *dst++ = color.r;
      *dst++ = color.g;
      *dst++ = color.b;
      *dst++ =     255;
    }}
}


void
image::
copy_rgb(const uint8_t*  src) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  auto  dst = get_rgba_pointer(0,0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ =    255;
    }}
}


void
image::
copy_rgba(const uint8_t*  src) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  auto  dst = get_rgba_pointer(0,0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  r = *src++;
      auto  g = *src++;
      auto  b = *src++;
      auto  a = *src++;

        if(!a)
        {
          *dst++ = 0;
          *dst++ = 0;
          *dst++ = 0;
          *dst++ = 0;
        }

      else
        {
          *dst++ = r;
          *dst++ = g;
          *dst++ = b;
          *dst++ = a;
        }
    }}
}


void
image::
copy_gray(const uint8_t*  src) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  auto  dst = get_rgba_pointer(0,0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  l = *src++;

      *dst++ =   l;
      *dst++ =   l;
      *dst++ =   l;
      *dst++ = 255;
    }}
}


void
image::
copy_gray_with_alpha(const uint8_t*  src) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  auto  dst = get_rgba_pointer(0,0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  l = *src++;
      auto  a = *src++;

        if(!a)
        {
          l = 0;
        }

      
      *dst++ = l;
      *dst++ = l;
      *dst++ = l;
      *dst++ = a;
    }}
}




image_header
image::
make_image_header() const noexcept
{
  return image_header(get_width(),get_height());
}


image_data
image::
make_image_data() const noexcept
{
  auto  ihdr = make_image_header();

  auto  src = binary(m_data,4*m_width*m_height);

  auto  filtered = src.get_filtered(ihdr);

  return image_data(filtered.get_compressed());
}




void
image::
blend(const image&  src, const image&  dst, uint32_t  x_offset, uint32_t  y_offset) noexcept
{
  int  w = src.get_width() ;
  int  h = src.get_height();

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  src_p = src.get_rgba_pointer(x,y);

      auto  dst_p = &dst.m_data[4*dst.m_width*(y_offset+y)+4*(x_offset+x)];

      uint8_t  src_r = src_p[0];
      uint8_t  src_g = src_p[1];
      uint8_t  src_b = src_p[2];
      uint8_t      a = src_p[3];

      uint8_t&  dst_r = dst_p[0];
      uint8_t&  dst_g = dst_p[1];
      uint8_t&  dst_b = dst_p[2];

        if(a == 255)
        {
          dst_r = src_r;
          dst_g = src_g;
          dst_b = src_b;
        }

      else
        if(a > 0)
        {
          dst_r = ((src_r-dst_r)*a>>8)+dst_r;
          dst_g = ((src_g-dst_g)*a>>8)+dst_g;
          dst_b = ((src_b-dst_b)*a>>8)+dst_b;
        }
    }}
}


void
image::
copy(const image&  src, const image&  dst, uint32_t  x_offset, uint32_t  y_offset) noexcept
{
  int  w = src.get_width() ;
  int  h = src.get_height();

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  src_p = src.get_rgba_pointer(x,y);

      auto  dst_p = &dst.m_data[4*dst.m_width*(y_offset+y)+4*(x_offset+x)];

      dst_p[0] = src_p[0];
      dst_p[1] = src_p[1];
      dst_p[2] = src_p[2];
      dst_p[3] = src_p[3];
    }}
}




}




