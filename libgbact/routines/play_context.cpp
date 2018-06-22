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

      m_system_message.set_base_point(real_point(view_off.x+g_screen_width/2,view_off.y+g_screen_height/2));

      m_system_message.set_string("PRESS [ Z or ENTER ] KEY");

      m_system_message.align_center();

      m_system_message.show();

      g_object_space.append(m_system_message);

      add_pc(1);
      break;
  case(1):
        if(g_modified_input.test_p_button() &&
           g_input.test_p_button())
        {
//          system_message.set_string("STAGE 0");

//          system_message.align_right();

          add_pc(1);
        }
      break;
  case(2):
        m_system_message.die();

        new(&m_lady) gbact::characters::lady;

          {
            auto   mon = new characters::lady_monitor(m_lady,0,0);


            m_lady.set_base_point(100,100);

            g_character_space.append(m_lady);

            g_character_space.append_with_deleter(*new characters::wall(30,80));
            g_character_space.append_with_deleter(*new characters::wall(60,80));
            g_character_space.append_with_deleter(*new characters::wall(180,100));
            g_character_space.append_with_deleter(*new characters::wall(210,100));
            g_character_space.append_with_deleter(*new characters::wall(240,100));

            g_object_space.append_with_deleter(*mon);
          }


        m_set_meat_timer = false;

        add_pc(1);
  case(3):
        if(m_pausing)
        {
            if(g_modified_input.test_start_button() &&
               g_input.test_start_button())
            {
              m_pausing = false;
            }
        }

      else
        {
            if(g_modified_input.test_start_button() &&
                        g_input.test_start_button())
            {
//              m_pausing = true;

              auto&  v = characters::character::m_debug;

              v = !v;
            }

          else
            {
                if(!m_meat.is_alive())
                {
                    if(!m_set_meat_timer)
                    {
                      m_next_meat_time = g_time+4000;

                      m_set_meat_timer = true;
                    }

                  else
                    {
                        if(g_time >= m_next_meat_time)
                        {
                          m_set_meat_timer = false;

                          new(&m_meat) characters::meat;

                          m_meat.set_base_point(gbact::g_square_size*5,gbact::g_square_size*3);

                          g_character_space.append(m_meat);
                        }
                    }
                }


              g_object_space.update();
              g_character_space.update();

              g_character_space.detect_collision();

              g_board_view.chase_object(m_lady,4);

                if(!m_lady.is_alive())
                {
                  set_pc(4);
                }
            }
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




