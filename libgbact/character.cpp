#include"libgbact/character.hpp"




namespace gbact{
namespace characters{


images::image
g_image;


character::
character() noexcept:
image_object(g_image,rectangle())
{
}




void
character::
do_when_collided(object&  other_side, spaces::position  position) noexcept
{
  auto&  area = other_side.get_area();

      if(position == spaces::position::left)
      {
        set_left(area.right);

        set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::right)
      {
        set_right(area.left);

        set_kinetic_energy_x(0);
      }

    else
      if(position == spaces::position::top)
      {
        set_top(area.bottom);

        set_kinetic_energy_y(0);
      }

    else
      if(position == spaces::position::bottom)
      {
        set_bottom(area.top);

        set_kinetic_energy_y(0);

        be_landing();
      }
}


void
character::
render(images::image&  dst) const noexcept
{
  auto  rect = get_rectangle();

  dst.draw_rectangle_safely(colors::red,rect.x,rect.y,rect.w,rect.h);
}




}}




