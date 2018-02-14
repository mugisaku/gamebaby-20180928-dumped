#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




image::
image(int  w, int  h) noexcept
{
  resize(w,h);
}




void
image::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_pixels.resize(w*h);

  fill();
}




uint32_t
image::
get_pixel_color(int  x, int  y, palette const&  pal) const noexcept
{
  return pal.get_color(get_const_pixel(x,y).index);
}


void
image::
fill(pixel  pix) noexcept
{
  std::fill(m_pixels.begin(),m_pixels.end(),pix);
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
add(int  r, int  g, int  b) noexcept
{
    for(auto&  pix: m_pixels)
    {
        if(pix.index)
        {
          auto  new_r = clamp(pix.index.get_r()+r);
          auto  new_g = clamp(pix.index.get_g()+g);
          auto  new_b = clamp(pix.index.get_b()+b);

          pix.index = color_index(new_r,new_g,new_b);
        }
    }
}


void
image::
reverse_color() noexcept
{
    for(auto&  pix: m_pixels)
    {
        if(pix.index)
        {
          auto  new_r = (7-pix.index.get_r());
          auto  new_g = (7-pix.index.get_g());
          auto  new_b = (7-pix.index.get_b());

          pix.index = color_index(new_r,new_g,new_b);
        }
    }
}


}}





