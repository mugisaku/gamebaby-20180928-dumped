#include"libgbstd/piece.hpp"




namespace gbstd{
namespace pieces{




piece::
piece(const images::image&  img) noexcept:
m_image(&img)
{
}




void
piece::
react() noexcept
{
}


void
piece::
render(image&  dst) noexcept
{
  images::transfer(*m_image,m_src_point,m_width,m_height,image_cursor(dst,m_dst_point));
}




}}




