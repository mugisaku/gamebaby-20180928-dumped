#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




bullet::
bullet(character*  shooter, character*  target) noexcept:
m_shooter(shooter),
m_target(target)
{
  set_major(major_ids::bullet);
}




void
bullet::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
}


void
bullet::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}


void
bullet::
do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept
{
}


void
bullet::
do_when_collided(character&  other_side, positions::position  position) noexcept
{
  other_side.do_when_collided_with_bullet(*this,get_opposite(position));
}




void
bullet::
do_when_collided_with_square(square&  sq) noexcept
{
  die();
}


void
bullet::
do_when_changed_square(square*  new_sq, square*  old_sq) noexcept
{
}


void
bullet::
hit(character&  other_side) noexcept
{
    if(m_callback)
    {
      m_callback(*this,other_side);
    }
}




}}




