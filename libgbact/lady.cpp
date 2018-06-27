#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




lady::
lady() noexcept:
player(4)
{
  set_id(player_ids::lady);

  set_width( 24);
  set_height(32);

  set_offset(-12,-32);
}




void
lady::
reset() noexcept
{
  character::initialize();

  m_action = action::stand;

  reset_phase();

  set_life_level(4);

  set_kinetic_energy(0,0);

  blink(0);
}


void
lady::
do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept
{
}


void
lady::
do_when_collided_with_player(player&  other_side, positions::position  position) noexcept
{
    if((other_side.get_id() == player_ids::boy) && other_side.is_landing())
    {
        if(m_action != action::meet)
        {
          m_action = action::meet;

          set_life_level(5);

          set_kinetic_energy_y(-2);
        }
    }
}


void
lady::
do_when_collided_with_item(item&  other_side, positions::position  position) noexcept
{
    if((m_action == action::stand) ||
       (m_action == action::walk))
    {
        if(get_life_level() < 5)
        {
          add_life_level(1);

          m_action = action::eat;

          check_last_animated_time(0);

          reset_phase();
        }


      other_side.die();
    }
}




void
lady::
do_when_collided_with_square(boards::square&  sq) noexcept
{
    if(m_action == action::ladder)
    {
      m_action = action::stand;
    }
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
  case(action::meet  ): do_when_action_is_meet();break;
  case(action::ladder): do_when_action_is_ladder();break;
  case(action::orz   ): do_when_action_is_orz();break;
  case(action::kick  ): do_when_action_is_kick();break;
    }
}


void
lady::
do_when_this_is_floating() noexcept
{
    if(g_input.test_n_button())
    {
      m_action = action::orz;

      set_life_level(0);
    }
}




void
lady::
do_when_action_is_stand() noexcept
{
    if(g_input.test_p_button())
    {
      m_action = action::kick;


      auto  bullet = new characters::bullet(this,nullptr);

      bullet->set_callback([](characters::bullet&  bullet, character&  other_side){
        bullet.die();

        auto&  lady = *static_cast<characters::lady*>(bullet.get_shooter());

        lady.add_life_level(-1);
      });


      bullet->set_direction(get_direction());

      bullet->set_base_point(get_base_point()+real_point(is_facing_to_right()? 20:-20,-20));

      bullet->set_destructive_power((get_life_level() > 2)? 1:0);

      bullet->set_width( 4);
      bullet->set_height(4);

      bullet->set_life_time(1000);

      bullet->get_physics().disable();

      g_character_space.append_with_deleter(*bullet);
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
    if(g_input.test_up_button())
    {
      auto  sq = get_current_square();

        if(sq)
        {
          auto&  dat = sq->get_data<square_data>();

            if(dat.is_ladder())
            {
              auto  x = (static_cast<int>(get_base_point().x)/g_square_size*g_square_size)+(g_square_size/2);

              set_square_point_offset_y(-(g_square_size/2));

              set_base_point_x(x);

              m_action = action::ladder;

              get_physics().disable();

              hold();
            }
        }
    }

  else
    if(g_input.test_down_button())
    {
      auto  sq = get_current_square();

        if(sq)
        {
          auto&  dat = sq->get_link(boards::links::down)->get_data<square_data>();

            if(dat.is_ladder())
            {
              auto  x = (static_cast<int>(get_base_point().x)/g_square_size*g_square_size)+(g_square_size/2);

              set_square_point_offset_y(g_square_size/2);

              set_base_point_x(x);

              m_action = action::ladder;

              get_physics().disable();

              hold();
            }
        }
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

          die();
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

      set_square_point_offset_y(-(g_square_size/2));
    }

  else
    if(g_input.test_down_button())
    {
      add_base_point_y(1);

      set_square_point_offset_y(0);
    }

  else
    if(g_input.test_left_button())
    {
      m_action = action::stand;

      set_square_point_offset_y(-(g_square_size/2));

      get_physics().enable();

      unhold();
    }

  else
    if(g_input.test_right_button())
    {
      m_action = action::stand;

      set_square_point_offset_y(-(g_square_size/2));

      get_physics().enable();

      unhold();
    }

  else
    {
      return;
    }


  auto  sq = get_current_square();

    if(!sq)
    {
      die();
    }

  else
    if(!sq->get_data<square_data>().is_ladder())
    {
      m_action = action::stand;

      set_square_point_offset_y(-(g_square_size/2));

      get_physics().enable();

      unhold();
    }

  else
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
do_when_action_is_meet() noexcept
{
    if(check_last_animated_time(400))
    {
        if(get_phase() > 2)
        {
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

      set_rendering_offset(point(-12,-46));

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
      set_rendering_offset(point(-12,-46));
      break;
  case(action::turn):
  case(action::meet):
      src_point = point(48*0,48*1);

      rect.w = 24;
      set_rendering_offset(point(-12,-46));
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
      set_rendering_offset(point(-24,-46));
      break;
  case(action::eat):
      src_point = (phase == 0)? point(48*0,48*2)
                 :              point(48*1,48*2);

      rect.w = 24;
      set_rendering_offset(point(-12,-46));
      break;
  case(action::ladder):
      src_point = (phase == 0)? point(48*3,48*1)
                 :              point(48*4,48*1);

      rect.w = 24;
      set_rendering_offset(point(-12,-46));
      break;
    }


    if(get_direction() == direction::left)
    {
      rect.w = -rect.w;
    }


  set_image_rectangle(rect);
}




}}




