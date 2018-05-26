#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




void
greeting_sphere::
initialize() noexcept
{
  auto&  chr = get_character();

  chr.set_kind_code(kind_codes::bullet);

  chr.set_width( 16);
  chr.set_height(40);

  chr.set_offset(point(-8,-40));

  set_time(g_time+1000);
}




void
greeting_sphere::
do_when_collided_with_player(player&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

}


void
greeting_sphere::
do_when_collided_with_object(object&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();
}


void
greeting_sphere::
update_parameter() noexcept
{
  auto&  chr = get_character();

    if(g_time >= get_time())
    {
      chr.need_to_remove();
    }
}


void
greeting_sphere::
update_image() noexcept
{
}




}}




