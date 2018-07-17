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
  return m_lighting? point(24,0):point();
}


void
square::
print() const noexcept
{
  printf("square{ index{%2d,%2d}, }",m_index.x,m_index.y);
}




