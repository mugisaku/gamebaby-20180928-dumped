#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




player::
player() noexcept
{
  g_space.get_environment().set_gravitation(0.2);
  g_space.get_environment().set_fluid_kinetic_energy(real_point(0.0,0.0));
//    g_space.get_environment().set_fluid_viscosity(0.08);

  set_width( 24);
  set_height(48);

  set_rendering_offset(point(-12,-48));

  set_offset(point(-12,-48));
}




void
player::
move(direction  d, double  walk_value, double  run_value) noexcept
{
  auto  ene = get_kinetic_energy();

//      if(is_landing())
    {
//          if(does_walk())
        {
            if(std::abs(ene.x) < 2)
            {
          do_walk();
              ene.x += walk_value;

              set_kinetic_energy(ene);
            }
        }
/*

      else
        if(does_run())
        {
            if(std::abs(m_kinetic_energy.x) < 4)
            {
              m_kinetic_energy.x += run_value;
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
*/
     }


  m_direction = d;
}


void
player::
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
player::
update() noexcept
{
       if(g_input.test_right_button()         ){        move(direction::right,1,2 );}
  else if(g_modified_input.test_right_button()){ready_to_run(direction::right     );}
  else if(g_input.test_left_button()          ){        move(direction::left,-1,-2);}
  else if(g_modified_input.test_left_button() ){ready_to_run(direction::left      );}
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


   set_kinetic_energy(ene);

   object::update();

   
}


void
player::
render(images::image&  dst) const noexcept
{
  sprite  spr;

  spr.src_image = &get_image();

  auto&  src_point = static_cast<point&>(spr.src_rectangle);

  spr.src_rectangle.w = 24;
  spr.src_rectangle.h = 48;

  spr.dst_point = get_base_point()+get_rendering_offset();

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
    }


    if(m_direction == direction::left)
    {
      spr.src_rectangle.w = -spr.src_rectangle.w;
    }


  spr.render(dst);


  character::render(dst);
}




}}




