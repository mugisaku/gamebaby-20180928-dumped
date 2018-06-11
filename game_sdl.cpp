#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/direction.hpp"
#include"sdl.hpp"
#include"libgbact/character.hpp"
#include"libgbact/block.hpp"
#include<cmath>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


constexpr int  screen_width  = 320;
constexpr int  screen_height = 320;


uint32_t  g_time = 0;

keyboard  g_previous_input;
keyboard  g_modified_input;
keyboard           g_input;


images::image
g_bg_image;


spaces::space
g_space;


namespace{


gbact::characters::lady
g_lady;


images::image
g_final_image;


programs::program
g_program;


boards::board
g_board;


boards::board_view
g_board_view;


static gbact::blocks::waterfall  wf;


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

  static spaces::text_object  system_message("",styles::a_white_based_text_style);

  static uint32_t  time;

  auto&  view_off = g_board_view.get_offset();

    switch(get_pc())
    {
  case(0):
      system_message.set_base_point(real_point(view_off.x+screen_width/2,view_off.y+screen_height/2));

      system_message.set_string("PRESS [ Z or ENTER ] KEY TO START GAME");

      system_message.align_center();

      g_space.append_object(system_message);

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
        system_message.need_to_remove();

        new(&g_lady) gbact::characters::lady;


        g_lady.set_base_point(real_point(30,120));

        g_space.append_object(g_lady,true);
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
              pausing = true;
            }

          else
            {
              g_space.update();

              g_space.detect_collision();
              g_space.step(g_board);

              g_board_view.chase_object(g_lady,4);
            }
        }
      break;
  case(4):
      system_message.set_base_point(real_point(view_off.x+screen_width/2,view_off.y+screen_height/2));

      system_message.set_string("YOU LOOSE");

      system_message.align_center();

      g_space.append_object(system_message);

      time = g_time+4000;

      set_pc(6);
      break;
  case(6):
      g_space.update();

      g_space.detect_collision();
      g_space.step(g_board);

      g_board_view.chase_object(g_lady,4);

        if(g_time >= time)
        {
          g_space.remove_all_object();
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

      wf.step();

      g_final_image.fill();

      g_board_view.render(g_final_image);
      g_space.render(g_board_view.get_offset(),g_final_image);

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
//                  "*上キーでジャンプ\n"
//                  "*下キーでしゃがむ\n"
//                  "*zまたはenterで、おじぎで攻撃\n"
                  "</pre>"
  );
#endif


  sdl::init(screen_width,screen_height,1.5);

  gbact::characters::g_image.load_from_png("__resources/lady_and.png");
  g_bg_image.load_from_png("__resources/bg.png");

  g_final_image = sdl::make_screen_image();

  g_program.push(ctx);


static gbact::blocks::brick  blk;

  g_board.build(14,14,24);

/*
  g_board.get_square(1,7).set_data(&blk);
  g_board.get_square(5,3).set_data(&blk);
  g_board.get_square(5,7).set_data(&blk);
  g_board.get_square(8,9).set_data(&blk);
  g_board.get_square(3,12).set_data(&blk);
  g_board.get_square(6,17).set_data(&blk);
  g_board.get_square(8,17).set_data(&blk);
  g_board.get_square(9,17).set_data(&blk);
  g_board.get_square(6,12).set_data(&blk);
  g_board.get_square(7,12).set_data(&blk);
  g_board.get_square(8,19).set_data(&blk);
  g_board.get_square(7,17).set_data(&blk);
  g_board.get_square(7,18).set_data(&blk);
  g_board.get_square(4,15).set_data(&blk);
  g_board.get_square(4,14).set_data(&blk);

    for(int  i = 0;  i < 17;  ++i)
    {
      g_board.get_square(12,2+i).set_data(&wf);
      g_board.get_square(13,2+i).set_data(&wf);
    }
*/


  g_board.put_to_around(&blk);


  g_board_view.set_source_image(g_bg_image);
  g_board_view.reset(g_board,screen_width,screen_height);


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




