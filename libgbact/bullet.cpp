#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




bullet::
bullet(player*  shooter, player*  target) noexcept:
m_shooter(shooter),
m_target(target)
{
  set_width( 16);
  set_height(16);

  set_offset(point(-8,-8));
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
do_when_collided_with_square(boards::square&  sq) noexcept
{
  need_to_remove();
}


void
bullet::
do_when_changed_square(boards::square*  new_sq, boards::square*  old_sq) noexcept
{
}




void
bullet::
update_core() noexcept
{
  character::update_core();

  set_kinetic_energy_y(0);
}


void
bullet::
update_graphics() noexcept
{
  character::update_graphics();


  set_image(g_image);

  rectangle  rect;

  auto&  src_point = static_cast<point&>(rect);

  rect.w = 24;
  rect.h = 24;

  src_point = point(24*7,0);

    if(get_direction() == direction::left)
    {
      rect.w = -rect.w;
    }


  set_image_rectangle(rect);

  set_rendering_offset(point(-12,-12));
}




}}




