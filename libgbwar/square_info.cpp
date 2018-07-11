#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"




void
square_info::
update_core() noexcept
{
}


void
square_info::
render(point  offset, image_cursor  cur) noexcept
{
  offset += get_base_point();

    if(m_target)
    {
      gbstd::string_form  sf;

      int  y = 0;


      sf("x: %2d, y: %2d",m_target->get_index().x,m_target->get_index().y);

      cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

      y += 16;


      sf("mv: %2d",m_target->get_mv());

      cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

      y += 16;
    }
}





