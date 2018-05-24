#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




void
character_data::
do_when_collided_with_object(spaces::object&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

  auto&  area = other_side.get_area();

      if(position == spaces::position::left)
      {
        chr.set_left(area.right);

        chr.set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::right)
      {
        chr.set_right(area.left);

        chr.set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::top)
      {
        chr.set_top(area.bottom);

        chr.set_kinetic_energy_y(0);
      }

    else
      if(position == spaces::position::bottom)
      {
        chr.set_bottom(area.top);

        chr.set_kinetic_energy_y(0);

        be_landing();
      }
}




void
character_data::
do_when_collided_with_character(character&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

  auto&  area = other_side.get_area();

      if(position == spaces::position::left)
      {
        chr.set_left(area.right);

        chr.set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::right)
      {
        chr.set_right(area.left);

        chr.set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::top)
      {
        chr.set_top(area.bottom);

        chr.set_kinetic_energy_y(0);
      }

    else
      if(position == spaces::position::bottom)
      {
        chr.set_bottom(area.top);

        chr.set_kinetic_energy_y(0);

        be_landing();
      }
}




}}




