#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/direction.hpp"
#include"sdl.hpp"
#include"libgbact/character.hpp"
#include<cmath>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


constexpr int  screen_width  = 288;
constexpr int  screen_height = 240;


uint32_t  g_time = 0;

keyboard  g_previous_input;
keyboard  g_modified_input;
keyboard           g_input;


images::image
g_bg_image;


spaces::space<gbact::characters::character>
g_character_space;


spaces::space<spaces::object>
g_object_space;


boards::board
g_board;


namespace{


images::image
g_final_image;


programs::program
g_program;


boards::board_view
g_board_view;


class
root_context: public programs::context
{
public:
  void  step() noexcept override;

} ctx;


void
root_context::
step() noexcept
{
  static bool  pausing;

  static gbact::characters::lady  lady;
  static gbact::characters::meat  meat;

  static spaces::text_object  system_message("",styles::a_white_based_text_style);

  static uint32_t  next_meat_time;

  static uint32_t  time;

  auto&  view_off = g_board_view.get_offset();

    switch(get_pc())
    {
  case(0):
      system_message.set_base_point(real_point(view_off.x+screen_width/2,view_off.y+screen_height/2));

      system_message.set_string("PRESS [ Z or ENTER ] KEY TO START GAME");

      system_message.align_center();

      system_message.show();

      g_object_space.append(system_message);

      add_pc(1);
      break;
  case(1):
        if(g_modified_input.test_p_button() &&
           g_input.test_p_button())
        {
//          system_message.set_string("STAGE 0");

//          system_message.align_right();

          sleep(1000);

          add_pc(1);
        }
      break;
  case(2):
        system_message.die();

        new(&lady) gbact::characters::lady;

          {
            auto   mon = new gbact::characters::lady_monitor(lady,0,0);


            lady.set_base_point(100,100);

            g_character_space.append(lady);

            g_character_space.append_with_deleter(*new gbact::characters::wall(30,80));
            g_character_space.append_with_deleter(*new gbact::characters::wall(60,80));
            g_character_space.append_with_deleter(*new gbact::characters::wall(180,100));
            g_character_space.append_with_deleter(*new gbact::characters::wall(210,100));
            g_character_space.append_with_deleter(*new gbact::characters::wall(240,100));

            g_object_space.append_with_deleter(*mon);
          }


        add_pc(1);
  case(3):
        if(pausing)
        {
            if(g_modified_input.test_start_button() &&
               g_input.test_start_button())
            {
              pausing = false;
            }
        }

      else
        {
            if(g_modified_input.test_start_button() &&
                        g_input.test_start_button())
            {
//              pausing = true;

              auto&  v = gbact::characters::character::m_debug;

              v = !v;
            }

          else
            {
                if(!meat.is_alive())
                {
                  static bool  set_timer;

                    if(!set_timer)
                    {
                      next_meat_time = g_time+4000;

                      set_timer = true;
                    }

                  else
                    {
                        if(g_time >= next_meat_time)
                        {
                          set_timer = false;

                          new(&meat) gbact::characters::meat;

                          meat.set_base_point(gbact::g_square_size*5,gbact::g_square_size*3);

                          g_character_space.append(meat);
                        }
                    }
                }


              g_object_space.update();
              g_character_space.update();

              g_character_space.detect_collision();

              g_board_view.chase_object(lady,4);

                if(!lady.is_alive())
                {
                  set_pc(4);
                }
            }
        }
      break;
  case(4):
      g_object_space.update();

      g_character_space.update();

      g_character_space.detect_collision();

      g_board_view.chase_object(lady,4);

        if(g_time >= time)
        {
          g_character_space.remove_all();
          set_pc(0);
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

  g_program.step(condev.time);


  static uint32_t  last;

    if(g_time >= last+60)
    {
      last = g_time;

      g_final_image.fill();

      g_board_view.render(image_cursor(g_final_image,point(0,48)));

      g_character_space.render(g_board_view.get_offset(),image_cursor(g_final_image,point(0,48)));
      g_object_space.render(point(),image_cursor(g_final_image,point(0,0)));

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


  sdl::init(screen_width,screen_height,1.5);

  gbact::characters::g_image.load_from_png("__resources/lady_and.png");
  g_bg_image.load_from_png("__resources/bg.png");

  g_final_image = sdl::make_screen_image();

  g_program.push(ctx);


  static auto  nul_sqdat = gbact::square_data::null();
  static auto  blk_sqdat = gbact::square_data::block();
  static auto   l0_sqdat = gbact::square_data::ladder0();
  static auto   l1_sqdat = gbact::square_data::ladder1();

  g_board.build(12,8,gbact::g_square_size,nul_sqdat);

  g_board.put_to_around(blk_sqdat);

  g_board.get_square(5,5).set_data(l0_sqdat);
  g_board.get_square(5,6).set_data(l1_sqdat);


  g_board_view.set_source_image(g_bg_image);
  g_board_view.reset(g_board,screen_width,screen_height-48);


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




