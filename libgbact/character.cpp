#include"libgbact/character.hpp"




namespace gbact{
namespace characters{


images::image
g_image;




bool
character::
check_last_animated_time(uint32_t  interval) noexcept
{
    if(g_time >= (m_last_animated_time+interval))
    {
      m_last_animated_time = g_time;

      ++m_phase;

      return true;
    }


  return false;
}


void
character::
blink(uint32_t  time) noexcept
{
  m_blinking_status.valid = true;

  m_blinking_status.end_time = g_time+time;
}


void
character::
do_when_collided(object&  other_side, positions::position  position) noexcept
{
    if(other_side.get_kind_code() == kind_codes::player)
    {
      do_when_collided_with_player(static_cast<player&>(other_side),position);
    }

  else
    if(other_side.get_kind_code() == kind_codes::bullet)
    {
      do_when_collided_with_bullet(static_cast<bullet&>(other_side),position);
    }

  else
    {
      do_when_collided_with_object(other_side,position);
    }
}


void
character::
update_core() noexcept
{
    if(m_blinking_status.valid)
    {
        if(g_time >= m_blinking_status.end_time)
        {
          m_blinking_status.valid = false;
        }
    }


  object::update_core();

  m_last_update_time = g_time;
}


void
character::
render(point  offset, images::image&  dst) noexcept
{
  ++m_rendering_count;

    if(m_visible && (!m_blinking_status.valid || (m_rendering_count&1)))
    {
      image_object::render(offset,dst);
    }


/*
  auto  rect = get_rectangle();

  dst.draw_rectangle_safely(colors::red,rect.x,rect.y,rect.w,rect.h);


  auto  sq = get_current_square();

    if(sq)
    {
      auto&  area = sq->get_area();

      dst.fill_rectangle(colors::blue,area.left,area.top,24,24);

      auto&  base_pt = get_base_point();

      dst.draw_vline(colors::red,base_pt.x   ,base_pt.y-32,64);
      dst.draw_hline(colors::red,base_pt.x-32,base_pt.y   ,64);
    }
*/
}




}}




