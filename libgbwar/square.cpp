#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"




void
square::
set_piece(piece*  p) noexcept
{
    if(m_piece)
    {
      m_piece->set_square(nullptr);
    }


  m_piece = p;

    if(p)
    {
      p->set_square(this);
    }
}


point
square::
get_image_base_point() const noexcept
{
  return (m_mv_consumption < 0)? point():point(24,0);
}




