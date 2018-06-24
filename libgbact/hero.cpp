#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




hero::
hero() noexcept:
player(4)
{
  set_width( 24);
  set_height(48);

  set_offset(point(-12,-48));
}




constexpr int  g_walk_value_max = 2;
constexpr int  g_walk_value     = 3;
constexpr int   g_run_value     = 4;
constexpr int   g_run_value_max = 6;


void
hero::
move_to_left() noexcept
{
  auto  ene = get_kinetic_energy();

    if(is_landing())
    {
        if(does_walk())
        {
          add_base_point(real_point(-1,0));

            if(ene.x > -g_walk_value_max)
            {
              ene.x += -g_walk_value;

              set_kinetic_energy(ene);
            }
        }

      else
        if(does_run())
        {
          add_base_point(real_point(-1,0));

            if(ene.x > -g_run_value_max)
            {
              ene.x += -g_run_value;

              set_kinetic_energy(ene);
            }
        }

      else
        if(m_ready_to_run)
        {
            if((g_time < m_running_limit_time) &&
               (m_running_direction == direction::left))
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

  else
    {
      add_base_point(real_point(-1,0));

        if(ene.x > -g_walk_value_max)
        {
          ene.x += -g_walk_value;

          set_kinetic_energy(ene);
        }
     }


  set_direction(direction::left);
}


void
hero::
move_to_right() noexcept
{
  auto  ene = get_kinetic_energy();

    if(is_landing())
    {
        if(does_walk())
        {
          add_base_point(real_point(1,0));

            if(ene.x < g_walk_value_max)
            {
              ene.x += g_walk_value;

              set_kinetic_energy(ene);
            }
        }

      else
        if(does_run())
        {
          add_base_point(real_point(1,0));

            if(ene.x < g_run_value_max)
            {
              ene.x += g_run_value;

              set_kinetic_energy(ene);
            }
        }

      else
        if(m_ready_to_run)
        {
            if((g_time < m_running_limit_time) &&
               (m_running_direction == direction::right))
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

  else
    {
      add_base_point(real_point(1,0));

        if(ene.x < g_walk_value_max)
        {
          ene.x += g_walk_value;

          set_kinetic_energy(ene);
        }
     }


  set_direction(direction::right);
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
    if(does_stand())
    {
      m_action = action::greeting;

      m_greeting_end_time = g_time+1000;

      static any_character  chr;


      new(&chr) greeting_sphere(this,nullptr);

      bool  flag = (get_direction() == direction::right);

      chr.m_character.set_base_point(get_base_point()+real_point(flag? 16:-16,-8));

      g_character_space.append(chr.m_character);

      chr.m_character.get_physics().disable();
    }
}




void
hero::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
    if((this != other_side.get_shooter()) && !is_blinking())
    {
      add_life_level(-1);

      knockback();
    }
}


void
hero::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
    if(!is_blinking())
    {
      add_life_level(-1);

      knockback();
    }
}


void
hero::
update_core() noexcept
{
  player::update_core();

    if(does_greeting())
    {
        if(g_time >= m_greeting_end_time)
        {
          do_stand();
        }
    }

  else
    {
           if(g_input.test_right_button()         ){move_to_right();}
      else if(g_modified_input.test_right_button()){ready_to_run(direction::right);}
      else if(g_input.test_left_button()          ){move_to_left();}
      else if(g_modified_input.test_left_button() ){ready_to_run(direction::left);}
      else if(g_input.test_p_button() && is_landing()){do_greeting();}
      else
        {
          do_stand();

          set_kinetic_energy_x(0);
        }


      auto  ene = get_kinetic_energy();

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
              add_base_point(real_point(0,-1));

              ene.y -= 6;

              do_stand();
            }
        }


      set_kinetic_energy(ene);
    }


    if(check_last_animated_time(160))
    {
        if(get_phase() > 3)
        {
          reset_phase();
        }
    }
}


void
hero::
update_graphics() noexcept
{
  player::update_graphics();


  set_image(g_image);

  rectangle  rect;

  auto&  src_point = static_cast<point&>(rect);

  rect.w = 24;
  rect.h = 48;


  auto  phase = get_phase();

    switch(m_action)
    {
  case(action::stand):
      src_point = (is_landing())? point( 0,0)
                 :                point(24,0);
      break;
  case(action::walk):
  case(action::run):
      src_point = (phase == 0)? point( 0,0)
                 :(phase == 1)? point(24,0)
                 :(phase == 2)? point( 0,0)
                 :              point(48,0);
      break;
  case(action::squat):
      src_point = point(24*3,0);
      break;
  case(action::greeting):
      src_point = point(24*4,0);
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




