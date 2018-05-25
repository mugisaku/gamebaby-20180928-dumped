#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




hero::
hero() noexcept
{
  g_space.get_environment().set_gravitation(0.2);
  g_space.get_environment().set_fluid_kinetic_energy(real_point(0.0,0.0));
//    g_space.get_environment().set_fluid_viscosity(0.08);
}




void
hero::
move(direction  d, double  walk_value, double  run_value) noexcept
{
  auto&  chr = get_character();

  auto  ene = chr.get_kinetic_energy();

    if(is_landing())
    {
        if(does_walk())
        {
            if(std::abs(ene.x) < 2)
            {
              ene.x += walk_value;

              chr.set_kinetic_energy(ene);
            }
        }

      else
        if(does_run())
        {
            if(std::abs(ene.x) < 4)
            {
              ene.x += run_value;

              chr.set_kinetic_energy(ene);
            }
        }

      else
        if(m_ready_to_run)
        {
            if((g_time < m_running_limit_time) &&
               (m_running_direction == d))
            {
              do_run();
            }

          else
            {
              m_ready_to_run = false;
            }
        }

      else
        {
          do_walk();
        }
     }


  set_direction(d);
}


void
hero::
ready_to_run(direction  d) noexcept
{
    if(is_landing() && !m_ready_to_run)
    {
      m_running_direction = d;

      m_running_limit_time = g_time+200;

      m_ready_to_run = true;
    }
}




void
hero::
do_greeting() noexcept
{
  m_action = action::greeting;

  m_greeting_end_time = g_time+1000;
}




void
hero::
initialize() noexcept
{
  auto&  chr = get_character();

  chr.set_kind_code(kind_codes::player);

  chr.set_width( 24);
  chr.set_height(48);

  chr.set_offset(point(-12,-48));
}


void
hero::
do_when_collided_with_bullet(bullet&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

}


void
hero::
do_when_collided_with_player(player&  other_side, spaces::position  position) noexcept
{
  auto&  chr = get_character();

  other_side.get_character().need_to_remove();
}


void
hero::
update_parameter() noexcept
{
  auto&  chr = get_character();

    if(does_greeting())
    {
        if(g_time >= m_greeting_end_time)
        {
          do_stand();
        }
    }

  else
    {
           if(g_input.test_right_button()         ){        move(direction::right,1,2 );}
      else if(g_modified_input.test_right_button()){ready_to_run(direction::right     );}
      else if(g_input.test_left_button()          ){        move(direction::left,-1,-2);}
      else if(g_modified_input.test_left_button() ){ready_to_run(direction::left      );}
      else if(g_input.test_p_button() && is_landing()){do_greeting();}
      else
        {
          do_stand();

          chr.set_kinetic_energy_x(0);
        }
    }


  auto  ene = chr.get_kinetic_energy();

    if(g_input.test_down_button())
    {
        if(is_landing())
        {
          ene.x = 0;

          do_squat();
        }
    }

  else
    if(g_input.test_up_button())
    {
        if(is_landing())
        {
          ene.y -= 5;

          do_stand();

          be_floating();
        }
    }


    if(g_time >= (m_last_animated_time+160))
    {
      m_last_animated_time = g_time;

        if(++m_phase > 3)
        {
          m_phase = 0;
        }
    }


  chr.set_kinetic_energy(ene);
}


void
hero::
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
  case(action::stand):
      src_point = (m_state == state::landing)? point( 0,0)
                 :                             point(24,0);
      break;
  case(action::walk):
  case(action::run):
      src_point = (m_phase == 0)? point( 0,0)
                 :(m_phase == 1)? point(24,0)
                 :(m_phase == 2)? point( 0,0)
                 :                point(48,0);
      break;
  case(action::squat):
      src_point = point(24*3,0);
      break;
  case(action::greeting):
      src_point = point(24*4,0);
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




