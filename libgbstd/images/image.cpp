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
}




uint32_t
image::
get_pixel_color(int  x, int  y, palette const&  pal) const noexcept
{
  return pal.get_color(get_const_pixel(x,y).index);
}


}}





