#include"libgbwar/piece.hpp"




void
piece_object::
update_core() noexcept
{
    if(gbstd::g_time >= m_last_time+200)
    {
      m_last_time = g_time;

        if(++m_pause_counter > 3)
        {
          m_pause_counter = 0;
        }
    }
}


void
piece_object::
render(point  offset, image_cursor  cur) noexcept
{
  offset += get_base_point();

    if(m_target)
    {
      int  x = 48*((m_pause_counter == 0)? 0
                  :(m_pause_counter == 1)? 1
                  :(m_pause_counter == 2)? 0
                  :                        2);

      images::overlay(piece::g_image,rectangle(x,0,24,24),cur+offset);
    }
}





