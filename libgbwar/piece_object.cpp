#include"libgbwar/piece.hpp"




void
piece_object::
update_core() noexcept
{
}


void
piece_object::
render(point  offset, image_cursor  cur) noexcept
{
  offset += get_base_point();

    if(m_target)
    {
      cur.fill_rectangle(colors::red,offset.x,offset.y,24,24);
    }
}





