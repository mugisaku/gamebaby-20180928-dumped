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

  static gbact::characters::lady  lady;

  static spaces::text_object  system_message("",styles::a_white_based_text_style);

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
            auto  meat = new gbact::characters::meat(200,48);
            auto  wall = new gbact::characters::wall(200,80);


            lady.set_base_point(30,120);

            auto  del = [](gbact::character*  ptr){delete ptr;};

            g_character_space.append(lady);
            g_character_space.append(*meat,del);
            g_character_space.append(*wall,del);

            g_object_space.append(*mon,[](spaces::object*  ptr){delete ptr;});
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
              pausing = true;
            }

          else
            {
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

      wf.step();

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
//                  "*上キーでジャンプ\n"
//                  "*下キーでしゃがむ\n"
//                  "*zまたはenterで、おじぎで攻撃\n"
                  "</pre>"
  );
#endif
  gbact::characters::character::m_debug = true;



  sdl::init(screen_width,screen_height,1.5);

  gbact::characters::g_image.load_from_png("__resources/lady_and.png");
  g_bg_image.load_from_png("__resources/bg.png");

  g_final_image = sdl::make_screen_image();

  g_program.push(ctx);


  static gbact::blocks::brick  blk;

  g_board.build(14,12,24);

  g_board.put_to_around(&blk);


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




