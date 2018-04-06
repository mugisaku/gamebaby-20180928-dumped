#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




image_cursor
image_cursor::
operator+(point  offset) const noexcept
{
  return image_cursor(*m_image,point(m_x_offset+offset.x,
                                     m_y_offset+offset.y));
}




void
image_cursor::
set_offset(int  x, int  y) noexcept
{
  m_x_offset = x;
  m_y_offset = y;
}


void
image_cursor::
add_offset(int  x, int  y) noexcept
{
  m_x_offset += x;
  m_y_offset += y;
}


}}





