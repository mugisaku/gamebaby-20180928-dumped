#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




wall::
wall(int  x, int  y) noexcept
{
  set_width( 48);
  set_height(48);
  set_offset(-24,-48);

  set_base_point(x,y);

  set_image(g_image);

  rectangle  rect;

  rect.x = 48*5;
  rect.y = 48*1;
  rect.w = 48;
  rect.h = 48;

  set_image_rectangle(rect);

  set_rendering_offset(point(-24,-48));
}




void
wall::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
}


void
wall::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
  block(other_side,position);
}


void
wall::
do_when_collided_with_item(item&  other_side, positions::position  position) noexcept
{
}




}}




