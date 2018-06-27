#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




boy::
boy() noexcept
{
  set_id(player_ids::boy);

  set_width( 24);
  set_height(48);
  set_offset(-12,-48);

  set_image(g_image);

  rectangle  rect;

  rect.x = 48*5;
  rect.y = 48*0;
  rect.w = 24;
  rect.h = 48;

  set_image_rectangle(rect);

  set_rendering_offset(point(-12,-48));
}




void
boy::
reset() noexcept
{
  character::initialize();

  reset_phase();

  set_kinetic_energy(0,0);

  blink(0);
}


void
boy::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
  knockback();
}


void
boy::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}


void
boy::
do_when_collided_with_item(item&  other_side, positions::position  position) noexcept
{
}




}}




