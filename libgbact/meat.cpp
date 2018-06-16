#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




meat::
meat() noexcept
{
  set_width(  8);
  set_height(24);

  set_offset(point(-4,-24));
  set_rendering_offset(point(-12,-24));

  get_physics().enable();


  set_image(g_image);

  rectangle  rect;

  rect.x = 48*5+24;
  rect.y = 48*0+24;
  rect.w = 24;
  rect.h = 24;

  set_image_rectangle(rect);
}




}}




