#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{
namespace images{




image_cursor
image_cursor::
operator+(point  offset) const noexcept
{
  return image_cursor(*m_image,point(m_rectangle.x+offset.x,
                                     m_rectangle.y+offset.y));
}




void
image_cursor::
set_offset(int  x, int  y) noexcept
{
  m_rectangle.x = x;
  m_rectangle.y = y;
}


void
image_cursor::
add_offset(int  x, int  y) noexcept
{
  m_rectangle.x += x;
  m_rectangle.y += y;
}


}}





