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
do_when_collided(object&  other_side, spaces::position  position) noexcept
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
render(images::image&  dst) noexcept
{
  ++m_rendering_count;

    if(m_visible && (!m_blinking_status.valid || (m_rendering_count&1)))
    {
      image_object::render(dst);
    }


  auto  rect = get_rectangle();

  dst.draw_rectangle_safely(colors::red,rect.x,rect.y,rect.w,rect.h);
}




void
character::
do_when_collided_with_object(spaces::object&  other_side, spaces::position  position) noexcept
{
  auto&  area = other_side.get_area();

      if(position == spaces::position::left)
      {
        set_left(area.right);

        set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::right)
      {
        set_right(area.left);

        set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::top)
      {
        set_top(area.bottom);

        set_kinetic_energy_y(0);
      }

    else
      if(position == spaces::position::bottom)
      {
        set_bottom(area.top);

        set_kinetic_energy_y(0);

        be_landing();
      }
}




}}




