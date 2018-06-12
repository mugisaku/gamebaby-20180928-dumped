#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




lady::
lady() noexcept:
player(5)
{
  g_space.get_environment().set_gravitation(0.2);

//  g_space.get_environment().set_fluid_kinetic_energy(real_point(0.0,));

//  g_space.get_environment().set_fluid_viscosity(0.08);

  set_kind_code(kind_codes::lady);

  set_width( 24);
  set_height(48);

  set_offset(point(-12,-48));
}




void
lady::
do_when_this_is_landing() noexcept
{
    switch(m_action)
    {
  case(action::stand ): do_when_action_is_stand();break;
  case(action::walk  ): do_when_action_is_walk();break;
  case(action::turn  ): do_when_action_is_turn();break;
  case(action::eat   ): do_when_action_is_eat();break;
  case(action::ladder): do_when_action_is_ladder();break;
  case(action::orz   ): do_when_action_is_orz();break;
  case(action::kick  ): do_when_action_is_kick();break;
    }
}


void
lady::
do_when_this_is_floating() noexcept
{
}




void
lady::
do_when_action_is_stand() noexcept
{
    if(g_input.test_p_button())
    {
      m_action = action::kick;
    }

  else
    if(g_input.test_right_button())
    {
        if(get_direction() == direction::right)
        {
          m_action = action::walk;
        }

      else
        {
          m_action = action::turn;
        }
    }

  else
    if(g_input.test_left_button())
    {
        if(get_direction() == direction::left)
        {
          m_action = action::walk;
        }

      else
        {
          m_action = action::turn;
        }
    }

  else
    if(  g_input.test_up_button() ||
       g_input.test_down_button())
    {
      m_action = action::ladder;
    }

  else
    if(g_input.test_n_button())
    {
      m_action = action::orz;

      set_life_level(0);
    }


  check_last_animated_time(0);

  reset_phase();
}


void
lady::
do_when_action_is_walk() noexcept
{
  constexpr int  add_amount = 3;

    if(g_input.test_right_button())
    {
        if(get_direction() == direction::right)
        {
          add_base_point_x(add_amount);
        }

      else
        {
          m_action = action::turn;
        }
    }

  else
    if(g_input.test_left_button())
    {
        if(get_direction() == direction::left)
        {
          add_base_point_x(-add_amount);
        }

      else
        {
          m_action = action::turn;
        }
    }

  else
    {
      add_kinetic_energy_x(0);

      m_action = action::stand;
    }


    if(check_last_animated_time(160))
    {
        if(get_phase() > 2)
        {
          ++m_stepping_phase;

          reset_phase();
        }
    }
}


void
lady::
do_when_action_is_turn() noexcept
{
    if(check_last_animated_time(200))
    {
      reset_phase();

      set_direction(get_opposite(get_direction()));

      m_action = action::stand;
    }
}


void
lady::
do_when_action_is_kick() noexcept
{
    if(check_last_animated_time(400))
    {
      add_life_level(-1);

      reset_phase();

      m_action = action::stand;
    }
}

void
lady::
do_when_action_is_orz() noexcept
{
    if(check_last_animated_time(1000))
    {
        if(get_phase() > 2)
        {
          reset_phase();

          need_to_remove();
        }
    }
}


void
lady::
do_when_action_is_ladder() noexcept
{
    if(g_input.test_up_button())
    {
      add_base_point_y(-1);
    }

  else
    if(g_input.test_down_button())
    {
      add_base_point_y(1);
    }

  else
    {
      m_action = action::stand;
    }


    if(check_last_animated_time(400))
    {
        if(get_phase() > 1)
        {
          reset_phase();
        }
    }
}


void
lady::
do_when_action_is_eat() noexcept
{
    if(check_last_animated_time(400))
    {
        if(get_phase() > 2)
        {
          reset_phase();

          m_action = action::stand;
        }
    }
}




void
lady::
update_core() noexcept
{
  player::update_core();

    if(is_landing())
    {
      do_when_this_is_landing();
    }

  else
    if(is_floating())
    {
      do_when_this_is_floating();
    }
}


void
lady::
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
      src_point = point(48*0,0);

      rect.w = 24;

      set_rendering_offset(point(-12,-44));

      break;
  case(action::walk):
        if(m_stepping_phase&1)
        {
          src_point = (phase == 0)? point(48*1,0)
                     :              point(48*2,0);
        }

      else
        {
          src_point = (phase == 0)? point(48*3,0)
                     :              point(48*4,0);
        }

      rect.w = 24;
      set_rendering_offset(point(-12,-44));
      break;
  case(action::turn):
      src_point = point(48*0,48*1);

      rect.w = 24;
      set_rendering_offset(point(-12,-44));
      break;
  case(action::kick):
      src_point = point(48*2,48*2);

      rect.w = 48;
      set_rendering_offset(point(-24,-44));
      break;
  case(action::orz):
      src_point = (phase == 0)? point(48*3,48*2)
                 :              point(48*4,48*2);

      rect.w = 48;
      set_rendering_offset(point(-24,-44));
      break;
  case(action::eat):
      src_point = (phase == 0)? point(48*0,48*2)
                 :              point(48*1,48*2);

      rect.w = 24;
      set_rendering_offset(point(-12,-44));
      break;
  case(action::ladder):
      src_point = (phase == 0)? point(48*3,48*1)
                 :              point(48*4,48*1);

      rect.w = 24;
      set_rendering_offset(point(-12,-44));
      break;
    }


    if(get_direction() == direction::left)
    {
      rect.w = -rect.w;
    }


  set_image_rectangle(rect);
}




}}




