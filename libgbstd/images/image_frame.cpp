#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
image_frame::
fill(pixel  pix) const noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      get_pixel(x,y) = pix;
    }}
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
image_frame::
add(int  r, int  g, int  b) const noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  pix = get_pixel(x,y);

        if(pix.color_index)
        {
          auto  new_r = clamp(pix.color_index.get_r()+r);
          auto  new_g = clamp(pix.color_index.get_g()+g);
          auto  new_b = clamp(pix.color_index.get_b()+b);

          pix.color_index = color_index(new_r,new_g,new_b);
        }
    }}
}


void
image_frame::
reverse_color() const noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  pix = get_pixel(x,y);

        if(pix.color_index)
        {
          auto  new_r = (7-pix.color_index.get_r());
          auto  new_g = (7-pix.color_index.get_g());
          auto  new_b = (7-pix.color_index.get_b());

          pix.color_index = color_index(new_r,new_g,new_b);
        }
    }}
}


}}





