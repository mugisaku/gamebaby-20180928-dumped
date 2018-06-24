#include"libgbact/routine.hpp"




namespace gbact{
namespace routines{




void
play_context::
clean() noexcept
{
  m_chooser_context.clean();

  g_object_space.remove_all();
  g_character_space.remove_all();
}


void
play_context::
callback(indication_context&  ctx, play_context*  play) noexcept
{
}


void
play_context::
step() noexcept
{
  auto&  view_off = g_board_view.get_offset();

    switch(get_pc())
    {
  case(0):
      g_board_view_validity.enable();
      g_character_space_validity.enable();
      g_object_space_validity.enable();

      m_lady_monitor = characters::lady_monitor(m_lady,0,0);

      g_object_space.append(m_lady_monitor);

      add_pc(1);
      break;
  case(1):
      m_lady = characters::lady();
      m_boy  =  characters::boy();

      m_lady.set_base_point(100,120);
      m_boy.set_base_point(200,120);

      g_character_space.append(m_lady);
      g_character_space.append(m_boy);

      add_pc(1);
      break;
  case(2):
      g_object_space.update();
      g_character_space.update();

      g_character_space.detect_collision();

      g_board_view.chase_object(m_lady,4);

        if(!m_lady.is_alive())
        {
          m_boy.die();

          set_pc(4);
        }
      break;
  case(4):
        if(!g_input.test_p_button())
        {
          m_chooser_context.initialize({
            "RESTART",
            "EXIT",
          });


          call(m_chooser_context);

          add_pc(1);
        }
      break;
  case(5):
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




