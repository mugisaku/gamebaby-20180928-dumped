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


images::image
g_misc_image;


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
sleep_context: public programs::context
{
  uint32_t  m_time;

public:
  sleep_context(uint32_t  t=0) noexcept: m_time(g_time+t){}

  void  operator()(uint32_t  t) noexcept
  {
    m_time = g_time+t;

    g_program.push(*this);
  }

  void  step() noexcept override
  {
      if(g_time >= m_time)
      {
        halt();
      }
  }

} sleep_ctx;


class
root_context: public programs::context
{
public:
  void  step() noexcept override;

} ctx;


class
edit_context: public programs::context
{
public:
  void  step() noexcept override;

} edit_ctx;


class
play_context: public programs::context
{
public:
  void  step() noexcept override;

} play_ctx;


void
root_context::
step() noexcept
{
  static int  index;

  static spaces::image_object*  cursor;

    switch(get_pc())
    {
  case(0):
      index = 0;

      g_object_space_validity.enable();
      g_character_space_validity.disable();
      g_board_view_validity.disable();

        {
          auto  edit = new spaces::text_object("EDIT",styles::a_white_based_text_style);
          auto  play = new spaces::text_object("PLAY",styles::a_white_based_text_style);

          cursor = new spaces::image_object(g_misc_image,rectangle(0,0,24,24),point(-28,4));

          edit->set_base_point(80, 80);
          play->set_base_point(80,104);
          cursor->set_base_point(80,80);

          g_object_space.append_with_deleter(*edit);
          g_object_space.append_with_deleter(*play);
          g_object_space.append_with_deleter(*cursor);
        }


      add_pc(1);
      break;
  case(1):
        if(g_modified_input.test_up_button() && g_input.test_up_button() && index)
        {
          --index;

          cursor->add_base_point_y(-24);
        }

      else
        if(g_modified_input.test_down_button() && g_input.test_down_button() && (index < 1))
        {
          ++index;

          cursor->add_base_point_y(24);
        }

      else
        if(g_modified_input.test_p_button() && g_input.test_p_button())
        {
          g_object_space.remove_all();

            switch(index)
            {
          case(0):
              break;
          case(1):
              g_program.push(play_ctx);
              break;
            }
        }
      break;
    }
}


void
edit_context::
step() noexcept
{
}


void
play_context::
step() noexcept
{
  static bool  pausing;

  static gbact::characters::lady  lady;
  static gbact::characters::meat  meat;

  static spaces::text_object  system_message("",styles::a_white_based_text_style);

  static bool  set_meat_timer;

  static uint32_t  next_meat_time;

  static uint32_t  time;

  auto&  view_off = g_board_view.get_offset();

    switch(get_pc())
    {
  case(0):
      g_board_view_validity.enable();
      g_character_space_validity.enable();
      g_object_space_validity.enable();

      system_message.set_base_point(real_point(view_off.x+screen_width/2,view_off.y+screen_height/2));

      system_message.set_string("PRESS [ Z or ENTER ] KEY");

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

          sleep_ctx(1000);

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


        set_meat_timer = false;

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
                    if(!set_meat_timer)
                    {
                      next_meat_time = g_time+4000;

                      set_meat_timer = true;
                    }

                  else
                    {
                        if(g_time >= next_meat_time)
                        {
                          set_meat_timer = false;

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
                  g_character_space.remove_all();

                  set_pc(0);
                }
            }
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

  g_program.step();


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


  sdl::init(screen_width,screen_height,1.5);

  gbact::characters::g_image.load_from_png("__resources/lady_and.png");
  g_bg_image.load_from_png("__resources/bg.png");
  g_misc_image.load_from_png("__resources/misc.png");

  g_final_image = sdl::make_screen_image();

  g_program.push(ctx);


  static auto  nul_sqdat = gbact::square_data::null();
  static auto  blk_sqdat = gbact::square_data::block();
  static auto   l0_sqdat = gbact::square_data::ladder0();
  static auto   l1_sqdat = gbact::square_data::ladder1();

  g_board.build(12,8,gbact::g_square_size,nul_sqdat);

  g_board.put_to_around(blk_sqdat);

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




