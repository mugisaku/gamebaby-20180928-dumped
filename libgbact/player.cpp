#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




void
player::
set_life_level(int  v) noexcept
{
  m_life_level = v;
}


void
player::
add_life_level(int  v) noexcept
{
  m_life_level += v;

    if(m_life_level <= 0)
    {
      m_life_level = 0;

      do_when_ran_out_life();
    }
}


void
player::
knockback() noexcept
{
  add_base_point(real_point((get_direction() == direction::left)? 8:-8,-4));

  add_kinetic_energy(real_point((get_direction() == direction::left)? 16:-16,-4));

  be_floating();

  blink(2000);
}


void
player::
exempt(uint32_t  time) noexcept
{
  m_exemption_status.valid = true;

  m_exemption_status.end_time = g_time+time;
}


void
player::
do_when_ran_out_life() noexcept
{
  need_to_remove();
}


void
player::
update_core() noexcept
{
  character::update_core();

    if(m_exemption_status.valid)
    {
        if(g_time >= m_exemption_status.end_time)
        {
          m_exemption_status.valid = false;
        }
    }
}


}}




