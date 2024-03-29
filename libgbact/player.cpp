#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




player::
player(int  life) noexcept:
m_life_level(life)
{
  set_major(major_ids::player);

  get_physics().enable();
}




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
  add_base_point(real_point((get_direction() == direction::left)? 2:-2,-4));

  set_kinetic_energy(real_point((get_direction() == direction::left)? 2:-2,-4));

  be_floating();

  blink(2000);
}




void
player::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
}


void
player::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}


void
player::
do_when_collided_with_item(item&  other_side, positions::position  position) noexcept
{
}


void
player::
do_when_collided(character&  other_side, positions::position  position) noexcept
{
  other_side.do_when_collided_with_player(*this,get_opposite(position));
}




void
player::
do_when_ran_out_life() noexcept
{
}


void
player::
do_when_changed_square(square*  new_sq, square*  old_sq) noexcept
{
}


}}




