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




