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
step() noexcept
{
  auto&  view_off = g_board_view.get_offset();

    switch(get_pc())
    {
  case(0):
      g_board_view_validity.enable();
      g_character_space_validity.enable();
      g_object_space_validity.enable();

      add_pc(1);
      break;
  case(1):
        m_lady = gbact::characters::lady();

        m_lady_monitor = characters::lady_monitor(m_lady,0,0);

        m_lady.set_base_point(100,100);

        g_character_space.append(m_lady);

        g_object_space.append(m_lady_monitor);

        add_pc(1);
      break;
  case(2):
      g_object_space.update();
      g_character_space.update();

      g_character_space.detect_collision();

      g_board_view.chase_object(m_lady,4);

        if(!m_lady.is_alive())
        {
          set_pc(4);
        }
      break;
  case(4):
      new(&m_chooser_context) routines::chooser_context({
        "RESTART",
        "EXIT",
      });


      call(m_chooser_context);

      add_pc(1);
      break;
  case(5):
        if(get_end_value().get_integer() == 0)
        {
          set_pc(0);
        }

      else
        {
          end();
        }
      break;
    }
}




}}




