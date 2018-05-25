#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




void
bullet::
initialize() noexcept
{
  auto&  chr = get_character();

  chr.set_kind_code(kind_codes::bullet);

  chr.set_width( 24);
  chr.set_height(24);

  chr.set_offset(point(-12,-12));
}




void
bullet::
do_when_collided_with_player(player&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

}




void
bullet::
update_parameter() noexcept
{
  auto&  chr = get_character();

}


void
bullet::
update_image() noexcept
{
  auto&  chr = get_character();

  chr.set_image(g_image);

  rectangle  rect;

  auto&  src_point = static_cast<point&>(rect);

  rect.w = 24;
  rect.h = 24;

  src_point = point(24*7,0);

    if(m_direction == direction::left)
    {
      rect.w = -rect.w;
    }


  chr.set_image_rectangle(rect);

  chr.set_rendering_offset(point(-12,-12));
}




}}




