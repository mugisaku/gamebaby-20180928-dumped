#include"libgbwar/piece.hpp"
#include"libgbwar/board.hpp"




void
piece::
set_square(square*  sq) noexcept
{
  m_square = sq;

    if(sq)
    {
      m_object.set_base_point(sq->get_index()*square::size);
    }
}





