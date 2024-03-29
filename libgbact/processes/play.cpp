#include"libgbact/process.hpp"




namespace gbact{
namespace processes{




void
play::
clean() noexcept
{
  m_chooser.clean();

  clean_stage();

  gbstd::g_screen_object_space.remove_all();
  g_object_space.remove_all();
  g_character_space.remove_all();
}


void
play::
callback(programs::indication&  ind, play*  play_prc) noexcept
{
}


void
play::
load_stage() noexcept
{
  auto&  stg = g_stage_table[g_stage_index];

  stg.restore(m_character_set);

  m_character_set.m_lady.reset();
  m_character_set.m_boy.reset();

  g_character_space.append(m_character_set.m_lady);
  g_character_space.append(m_character_set.m_boy);

    for(auto&  meat: m_character_set.m_meats)
    {
      g_character_space.append(meat);
    }


    for(auto&  wall: m_character_set.m_walls)
    {
      g_character_space.append(wall);
    }
}


void
play::
clean_stage() noexcept
{
  m_character_set.m_meats.clear();
  m_character_set.m_walls.clear();

  g_character_space.remove_all();
}


void
play::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      g_board_view.show();
      g_character_space.show();
      g_object_space.show();
      gbstd::g_screen_object_space.show();

      m_character_set.m_lady = characters::lady();
      m_character_set.m_boy  =  characters::boy();

      m_lady_monitor = characters::lady_monitor(m_character_set.m_lady,0,0);

      gbstd::g_screen_object_space.append(m_lady_monitor);

      add_pc(1);
      break;
  case(1):
      clean_stage();
       load_stage();

      add_pc(1);
      break;
  case(2):
        if(gbstd::g_input.test_start_button() &&
           gbstd::g_modified_input.test_start_button())
        {
          character::m_debug = !character::m_debug;
        }


      gbstd::g_screen_object_space.update();
      g_character_space.update();

      g_character_space.detect_collision();

      g_board_view.chase_object(m_character_set.m_lady,4);

        if(!m_character_set.m_lady.is_alive() ||
           m_character_set.m_lady.is_crying() ||
           m_character_set.m_lady.is_rejoicing())
        {
          m_timer = g_time+2000;

          set_pc(3);
        }
      break;
  case(3):
        if(g_time >= m_timer)
        {
          set_pc(4);
        }

      else
        {
          gbstd::g_screen_object_space.update();
          g_character_space.update();
        }
      break;
  case(4):
        if(!g_input.test_p_button())
        {
          m_chooser.initialize({
            "RESTART",
            "EXIT",
          },120,120);


          call(m_chooser);

          add_pc(1);
        }
      break;
  case(5):
      m_chooser.clean();

        if(get_end_value().is_integer())
        {
            if(get_end_value().get_integer() == 0)
            {
              set_pc(1);
            }

          else
            if(get_end_value().get_integer() == 1)
            {
              end();
            }
        }

      else
        {
          set_pc(4);
        }
      break;
    }
}




}}




