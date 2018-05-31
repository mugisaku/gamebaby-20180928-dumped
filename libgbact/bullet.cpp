#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




bullet::
bullet(player*  shooter, player*  target) noexcept:
m_shooter(shooter),
m_target(target)
{
  set_kind_code(kind_codes::bullet);

  set_width( 16);
  set_height(16);

  set_offset(point(-8,-8));
}




void
bullet::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}


void
bullet::
do_when_collided_with_object(object&  other_side, positions::position  position) noexcept
{
  need_to_remove();
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




