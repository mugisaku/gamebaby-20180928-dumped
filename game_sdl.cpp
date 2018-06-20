#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/direction.hpp"
#include"sdl.hpp"
#include"libgbact/character.hpp"
#include"libgbact/stage.hpp"
#include"libgbact/routine.hpp"
#include<cmath>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;
using namespace gbact;


constexpr int  g_screen_width  = 288;
constexpr int  g_screen_height = 240;


uint32_t  g_time = 0;

keyboard  g_previous_input;
keyboard  g_modified_input;
keyboard           g_input;


images::image
g_bg_image;


images::image
g_misc_image;


spaces::space<gbact::characters::character>
g_character_space;


spaces::space<spaces::object>
g_object_space;


boards::board
g_board;


namespace{


programs::program
g_program;


images::image
g_final_image;


boards::board_view
g_board_view;


class
validity
{
  bool  m_value=true;

public:
  operator bool() const noexcept{return m_value;}

  void   enable() noexcept{m_value =  true;}
  void  disable() noexcept{m_value = false;}

};


validity  g_object_space_validity;
validity  g_character_space_validity;
validity  g_board_view_validity;


class
edit_context: public programs::context
{
  routines::indication_context  m_indication_context;

public:
  void  step() noexcept override;

};


class
play_context: public programs::context
{
  routines::chooser_context  m_chooser_context;

  bool  m_pausing;

  characters::lady  m_lady;
  characters::meat  m_meat;

  spaces::text_object  m_system_message;

  bool  m_set_meat_timer;

  uint32_t  m_next_meat_time;

  uint32_t  m_time;

public:
  void  step() noexcept override;

};


class
root_context: public programs::context
{
  routines::chooser_context  m_chooser_context;

  edit_context  m_edit_context;
  play_context  m_play_context;

public:
  void  step() noexcept override;

};


void
edit_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      m_indication_context.initialize(rectangle(0,0,g_screen_width-24,g_screen_height-24));

      set_pc(1);
      break;
  case(1):
      call(m_indication_context);

      set_pc(2);
      break;
  case(2):
        {
          auto  kbd = get_end_value().get_keyboard();

            if(kbd.test_p_button())
            {
            }

          else
            if(kbd.test_n_button())
            {
            }
        }

      set_pc(1);
      break;
    }
}


void
root_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      new(&m_chooser_context) routines::chooser_context({
        "EDIT",
        "PLAY"
      });


      call(m_chooser_context);

      add_pc(1);
      break;
  case(1):
        switch(get_end_value().get_integer())
        {
      case(0): call(m_edit_context);break;
      case(1): call(m_play_context);break;
        }


      set_pc(0);
      break;
    }
};




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
                  g_character_space.remove_all();

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


void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  g_previous_input = g_input                  ;
                     g_input = condev.keyboard;

  g_modified_input = g_previous_input^g_input;

  g_time = condev.time;

    if(g_program)
    {
      g_program.step();
    }


  static uint32_t  last;

    if(g_time >= last+60)
    {
      last = g_time;

      g_final_image.fill();

        if(g_board_view_validity)
        {
          g_board_view.render(image_cursor(g_final_image,point(0,48)));
        }


        if(g_character_space_validity)
        {
          g_character_space.render(g_board_view.get_offset(),image_cursor(g_final_image,point(0,48)));
        }


        if(g_object_space_validity)
        {
          g_object_space.render(point(),image_cursor(g_final_image,point(0,0)));
        }


      sdl::update_screen(g_final_image);
    }
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("game - " __DATE__);
  set_description("<pre>"
                  "*キーボードで操作\n"
                  "*左右キーを押すと歩く\n"
                  "*はしご付近で上下キーを押すと、はしごを移動\n"
                  "*zまたはenterを押すとキック\n"
                  "*体力（肉アイコン）があれば、キックで特定の壁を壊せる\n"
                  "*xまたはctrlを押すとギブアップ\n"
//                  "*上キーでジャンプ\n"
//                  "*下キーでしゃがむ\n"
                  "</pre>"
  );
#endif


  sdl::init(g_screen_width,g_screen_height,1.5);

  gbact::characters::g_image.load_from_png("__resources/lady_and.png");
  g_bg_image.load_from_png("__resources/bg.png");
  g_misc_image.load_from_png("__resources/misc.png");

  g_final_image = sdl::make_screen_image();

  static root_context  root_ctx;

  g_program.push(root_ctx);

  using stage = gbact::stages::stage;

  g_board.build(12,8,gbact::g_square_size,stages::g_square_data_set[0]);

  g_board.put_to_around(stages::g_square_data_set[1]);

  g_board_view.set_source_image(g_bg_image);
  g_board_view.reset(g_board,g_screen_width,g_screen_height-48);


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  return 0;
}




