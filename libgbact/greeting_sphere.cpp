#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




greeting_sphere::
greeting_sphere(player*  shooter, player*  target) noexcept:
bullet(shooter,target)
{
  set_width( 16);
  set_height(40);

  set_offset(point(-8,-40));

  set_life_time(1000);
}




void
greeting_sphere::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}




}}




