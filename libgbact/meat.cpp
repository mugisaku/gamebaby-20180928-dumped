#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




meat::
meat(int  x, int  y) noexcept
{
  set_base_point(x,y);

  set_width( 24);
  set_height(24);

  set_offset(point(-12,-24));
  set_rendering_offset(point(-12,-24));

  get_physics().enable();
}


void
meat::
update_graphics() noexcept
{
  set_image(g_image);

  rectangle  rect;

  rect.x = 48*5;
  rect.y = 48*1+24;
  rect.w = 24;
  rect.h = 24;

  set_image_rectangle(rect);

}




}}




