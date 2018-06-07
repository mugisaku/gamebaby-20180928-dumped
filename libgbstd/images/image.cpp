#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




image::
image(int  w, int  h, std::initializer_list<int>  ls) noexcept
{
  resize(w,h);

  int  n = std::min(w*h,(int)ls.size());

  auto  src = ls.begin();
  auto  dst = m_pixels.begin();

    while(n--)
    {
      *dst++ = pixel(color(*src++));
    }
}


image::
image(int  w, int  h, std::initializer_list<color>  ls) noexcept
{
  resize(w,h);

  int  n = std::min(w*h,(int)ls.size());

  auto  src = ls.begin();
  auto  dst = m_pixels.begin();

    while(n--)
    {
      *dst++ = *src++;
    }
}




void
image::
resize(int  w, int  h) noexcept
{
    if((w == m_width ) &&
       (h == m_height))
    {
      return;
    }


  image  tmp(*this);

  m_width  = w;
  m_height = h;

  m_pixels.resize(w*h);


  int  old_w = tmp.get_width() ;
  int  old_h = tmp.get_height();

    for(int  y = 0;  y < old_h;  ++y){
    for(int  x = 0;  x < old_w;  ++x){
      set_pixel(tmp.get_pixel(x,y),x,y);
    }}
}




void
image::
set_pixel(pixel  pix, int  x, int  y, int  w, int  h) noexcept
{
    if(x < 0)
    {
      w += x    ;
           x = 0;
    }


    if(y < 0)
    {
      h += y    ;
           y = 0;
    }


    if((x+w) >= m_width)
    {
      w = m_width-x;
    }


    if((y+h) >= m_height)
    {
      h = m_height-y;
    }


    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      set_pixel(pix,x+xx,y+yy);
    }}
}




void
image::
fill(pixel  pix) noexcept
{
    for(auto&  dst: m_pixels)
    {
      dst = pix;
    }
}


namespace{
int
clamp(int  n) noexcept
{
  return (n <= 0)? 0:
         (n >= 7)? 7:n;
}
}


void
image::
add_rgb(int  r, int  g, int  b) noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  pix = get_pixel(x,y);

        if(pix.color)
        {
          auto  new_r = clamp(pix.color.get_r7()+r);
          auto  new_g = clamp(pix.color.get_g7()+g);
          auto  new_b = clamp(pix.color.get_b7()+b);

          pix.color = color(new_r,new_g,new_b);
        }
    }}
}


void
image::
reverse_rgb() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  pix = get_pixel(x,y);

        if(pix.color)
        {
          auto  new_r = (7-pix.color.get_r7());
          auto  new_g = (7-pix.color.get_g7());
          auto  new_b = (7-pix.color.get_b7());

          pix.color = color(new_r,new_g,new_b);
        }
    }}
}




}}





