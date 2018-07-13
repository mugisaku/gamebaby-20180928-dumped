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


      auto  total_mv_consum = m_target->get_total_mv_consumption();

        if(total_mv_consum)
        {
          sf("しょうひmv: %2d",total_mv_consum);

          cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

          y += 16;


          sf("きょり: %4d",m_target->get_distance());

          cur.draw_text(*sf,styles::a_white_based_text_style,offset.x,offset.y+y);

          y += 16;
       }
    }
}





