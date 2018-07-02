#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/text.hpp"
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


int  g_screen_width  = 288;
int  g_screen_height = 240;

int  g_board_width  = 12;
int  g_board_height = 10;

uint32_t  g_time = 0;

keyboard  g_previous_input;
keyboard  g_modified_input;
keyboard           g_input;


point
g_object_space_offset;


images::image
g_bg_image;


images::image
g_misc_image;


spaces::space<gbact::characters::character>
g_character_space;


spaces::space<spaces::object>
g_object_space;


spaces::space<spaces::object>
g_screen_object_space;


boards::board
g_board;


boards::board_view
g_board_view;


std::vector<gbact::stages::stage>
g_stage_table(1);


int
g_stage_index;


validity  g_screen_object_space_validity;
validity  g_object_space_validity;
validity  g_character_space_validity;
validity  g_board_view_validity;


namespace{


programs::program
g_program;


images::image
g_final_image;


class
root_context: public programs::context
{
  routines::chooser_context  m_chooser_context;

  routines::edit_context  m_edit_context;
  routines::play_context  m_play_context;

public:
  void  step() noexcept override;

};


void
root_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
        if(!g_input.test_p_button())
        {
          g_object_space_validity.enable();
          g_board_view_validity.enable();

          m_chooser_context.initialize({
            "EDIT",
            "PLAY"
          },120,120);


          call(m_chooser_context);

          add_pc(1);
        }
      break;
  case(1):
      m_chooser_context.clean();

        if(get_end_value().is_integer())
        {
            switch(get_end_value().get_integer())
            {
          case(0): call(m_edit_context);  set_pc(2);break;
          case(1): call(m_play_context);  set_pc(3);break;
            }
        }

      else
        {
          set_pc(0);
        }
      break;
  case(2):
      m_edit_context.clean();

      set_pc(0);
      break;
  case(3):
      m_play_context.clean();

      set_pc(0);
      break;
    }
};




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
          g_object_space.render(g_board_view.get_offset(),image_cursor(g_final_image,point(0,48)));
        }


        if(g_screen_object_space_validity)
        {
          g_screen_object_space.render(point(),image_cursor(g_final_image,point(0,0)));
        }


        if(characters::character::m_debug)
        {
          string_form  sf;

          g_final_image.draw_text(sf("chr %4d",g_character_space.get_main_list().size()),styles::a_white_based_text_style,0,0);
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

  g_board.build(g_board_width,g_board_height,gbact::g_square_size,stages::g_square_data_set[0]);

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




