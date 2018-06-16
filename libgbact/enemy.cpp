#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




enemy::
enemy(player*  target) noexcept:
player(4),
m_target(target)
{
  set_width( 24);
  set_height(48);

  set_offset(point(-12,-48));
}




void
enemy::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
    if((this != other_side.get_shooter()) && (m_action == action::attack) && !is_blinking())
    {
      add_life_level(-1);

      blink(2000);
    }
}


void
enemy::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
}




void
enemy::
update_core() noexcept
{
  player::update_core();

    if(m_target->get_base_point().x < get_base_point().x)
    {
      set_direction(direction::right);
    }

  else
    {
      set_direction(direction::left);
    }


    if(g_time >= (m_time+2000))
    {
      m_time = g_time;

        if(m_action == action::sleep)
        {
          static any_character  bch;

          new(&bch) bullet(this,m_target);

          bch.m_character.set_base_point(get_base_point()+real_point(0,-24));

          bch.m_bullet.set_direction(get_direction());

            if(get_direction() == direction::left)
            {
              bch.m_character.set_kinetic_energy_x(2);
            }

          else
            {
              bch.m_character.set_kinetic_energy_x(-2);
            }


          m_action = action::attack;

          g_character_space.append(bch.m_character);
        }

      else
        {
          m_action = action::sleep;
        }
    }
}


void
enemy::
update_graphics() noexcept
{
  player::update_graphics();

  set_image(g_image);

  rectangle  rect;

  auto&  src_point = static_cast<point&>(rect);

  rect.w = 24;
  rect.h = 48;

    switch(m_action)
    {
  case(action::sleep):
      src_point = point(24*5,0);
      break;
  case(action::attack):
      src_point = point(24*6,0);
      break;
    }


    if(get_direction() == direction::left)
    {
      rect.w = -rect.w;
    }


  set_image_rectangle(rect);

  set_rendering_offset(point(-12,-48));
}




}}




