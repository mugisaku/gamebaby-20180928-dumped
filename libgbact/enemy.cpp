#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




void
enemy::
initialize() noexcept
{
  auto&  chr = get_character();

  chr.set_kind_code(kind_codes::player);

  chr.set_width( 24);
  chr.set_height(48);

  chr.set_offset(point(-12,-48));
}




void
enemy::
do_when_collided_with_bullet(bullet&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

}


void
enemy::
do_when_collided_with_player(player&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();
}




void
enemy::
update_parameter() noexcept
{
  auto&  chr = get_character();

    if(m_target->get_base_point().x < chr.get_base_point().x)
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
          static character  bch;

          bch.set_data(new bullet(chr,*m_target));

          bch.set_base_point(chr.get_base_point());

          bch.get_data()->set_direction(get_direction());

            if(get_direction() == direction::left)
            {
              bch.set_kinetic_energy_x(-1);
            }

          else
            {
              bch.set_kinetic_energy_x(1);
            }


          m_action = action::attack;

          chr.get_space()->append_kinetic_object(bch);
        }

      else
        {
          m_action = action::sleep;
        }
    }
}


void
enemy::
update_image() noexcept
{
  auto&  chr = get_character();

  chr.set_image(g_image);

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


    if(m_direction == direction::left)
    {
      rect.w = -rect.w;
    }


  chr.set_image_rectangle(rect);

  chr.set_rendering_offset(point(-12,-48));
}




}}




