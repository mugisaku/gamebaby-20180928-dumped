#include"libgbwar/piece.hpp"




void
piece_info::
update_core() noexcept
{
}


void
piece_info::
render(point  offset, image_cursor  cur) noexcept
{
  offset += get_base_point();

    if(m_target)
    {
      gbstd::string_form  sf;

      int  y = 0;


      sf("%s",m_target->get_name().data());

      cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

      y += 16;


      sf("HP %2d/%2d",m_target->get_hp(),m_target->get_hp_max());

      cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

      y += 16;


      sf("MV %2d/%2d",m_target->get_mv(),m_target->get_mv_max());

      cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

      y += 16;
    }
}





