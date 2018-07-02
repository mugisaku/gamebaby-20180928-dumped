#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




int
g_count;


wall::
wall(int  x, int  y) noexcept
{
  ++g_count;

  set_minor(player_ids::wall);

  set_width( 24);
  set_height(48);
  set_offset(-12,-48);

  set_base_point(x,y);

  set_image(g_image);

  rectangle  rect;

  rect.x = 48*5;
  rect.y = 48*1;
  rect.w = 24;
  rect.h = 48;

  set_image_rectangle(rect);

  set_rendering_offset(point(-12,-48));
}




int
get_count() noexcept
{
  return g_count;
}


void
wall::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
    if(!other_side.get_destructive_power() || (other_side.get_minor() != bullet_ids::kick))
    {
      return;
    }


  freeze();

  die();

  --g_count;

  other_side.hit(*this);

  auto  img_rect = get_image_rectangle();

  constexpr int  size = 4;

  static uniform_rand  rand(1,5);

    for(int  y = 0;  y < 48;  y += size){
    for(int  x = 0;  x < 24;  x += size){
      auto  bullet = new characters::bullet(this,nullptr);

      bullet->set_width( size);
      bullet->set_height(size);

      bullet->set_kinetic_energy_y(                                        -rand());
      bullet->set_kinetic_energy_x(other_side.is_facing_to_right()? rand():-rand());

      bullet->get_physics().enable();

      bullet->set_base_point(get_base_point()+real_point(-12+x,-48+y));


      rectangle  rect;

      rect.x = img_rect.x+x;
      rect.y = img_rect.y+y;
      rect.w = size;
      rect.h = size;

      bullet->set_image(get_image());
      bullet->set_image_rectangle(rect);

      bullet->set_life_time(4000);

      g_character_space.append_with_deleter(*bullet);
    }}
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




