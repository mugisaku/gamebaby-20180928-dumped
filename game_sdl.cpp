#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/direction.hpp"
#include"sdl.hpp"
#include"libgbact/character.hpp"
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
title_image;


spaces::space
g_space;


class
block_object: public spaces::rectangle_object
{
public:
  using rectangle_object::rectangle_object;

};


class
death_object: public spaces::rectangle_object
{
public:
  using rectangle_object::rectangle_object;

  void  do_when_collided(spaces::object&  o, spaces::position  pos) noexcept override
  {
    o.need_to_remove();
  }

  void  render(image&  dst) const noexcept override{}

};


namespace{


gbact::characters::character
g_player_character;


gbact::characters::character
g_enemy_character;


images::image
g_final_image;


programs::program
g_program;


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

  static block_object  blocks[] = {
    block_object(rectangle(              0,240,screen_width, 16),colors::white),
    block_object(rectangle(              0,  0,          16,240),colors::white),
    block_object(rectangle(screen_width-16,  0,          16,240),colors::white),
  };

  static death_object  death_obj(rectangle(0,screen_height+48,screen_width,32),colors::red);

    switch(get_pc())
    {
  case(0):
      system_message.set_base_point(real_point(screen_width/2,screen_height/2));

      system_message.set_string("PRESS ANY KEY TO START GAME");

      system_message.align_center();

      g_space.append_object(system_message);

      add_pc(1);
      break;
  case(1):
        if(g_modified_input.test_all_button() &&
           g_input.test_all_button())
        {
          system_message.set_string("STAGE 0");

          system_message.align_right();

          sleep(1000);

          add_pc(1);
        }
      break;
  case(2):
        system_message.need_to_remove();

        g_player_character.set_data(new gbact::characters::hero);
        g_enemy_character.set_data(new gbact::characters::enemy(g_player_character));


        g_player_character.set_base_point(real_point(30,120));

        g_space.append_kinetic_object(g_player_character);

        g_enemy_character.set_base_point(real_point(180,160));

        g_space.append_kinetic_object(g_enemy_character);

          for(auto&  blk: blocks)
          {
            g_space.append_object(blk);
          }


        g_space.append_object(death_obj);

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

                if(!g_player_character.get_space())
                {
                  g_space.remove_all_object();

                  set_pc(4);
                }
            }
        }
      break;
  case(4):
      system_message.set_base_point(real_point(screen_width/2,screen_height/2));

      system_message.set_string("GAME OVER");

      system_message.align_center();

      g_space.append_object(system_message);

      sleep(2000);

      set_pc(0);
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

      g_space.render(g_final_image);

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
                  "*上キーでジャンプ\n"
                  "*下キーでしゃがむ\n"
                  "</pre>"
  );
#endif


  sdl::init(screen_width,screen_height);

  gbact::characters::g_image.load_from_png("__resources/__anigra.png");

  g_final_image = sdl::make_screen_image();

  g_program.push(ctx);


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




