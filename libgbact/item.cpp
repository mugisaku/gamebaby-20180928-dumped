#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




void
item::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
}


void
item::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}


void
item::
do_when_collided_with_item(item&  other_side, positions::position  position) noexcept
{
}


void
item::
do_when_collided(character&  other_side, positions::position  position) noexcept
{
  other_side.do_when_collided_with_item(*this,get_opposite(position));
}






}}




