#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




greeting_sphere::
greeting_sphere(player*  shooter, player*  target) noexcept:
bullet(shooter,target)
{
  set_kind_code(kind_codes::bullet);

  set_width( 16);
  set_height(40);

  set_offset(point(-8,-40));

  set_time(g_time+1000);
}




void
greeting_sphere::
do_when_collided_with_player(player&  other_side, spaces::position  position) noexcept
{
}


void
greeting_sphere::
do_when_collided_with_object(object&  other_side, spaces::position  position) noexcept
{
}


void
greeting_sphere::
update_core() noexcept
{
  bullet::update_core();

    if(g_time >= get_time())
    {
      need_to_remove();
    }
}




}}




