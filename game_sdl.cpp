#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/text.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/direction.hpp"
#include"libgbstd/io.hpp"
#include"sdl.hpp"
#include"libgbact/character.hpp"
#include"libgbact/stage.hpp"
#include"libgbact/process.hpp"
#include<cmath>


#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif


using namespace gbstd;
using namespace gbact;


int  g_screen_width  = 288;
int  g_screen_height = 240;

int  g_board_width  = 12;
int  g_board_height = 10;


point
g_object_space_offset;


images::image
g_bg_image;


spaces::space<gbact::characters::character>
g_character_space;


spaces::space<spaces::object>
g_object_space;


board
g_board;


board_view
g_board_view;


std::vector<gbact::stages::stage>
g_stage_table(1);


int
g_stage_index;


validity  g_board_view_validity;


namespace{


programs::program
g_program;


images::image
g_final_image;


class
root_process: public programs::process
{
  programs::chooser  m_chooser;

  gbact::processes::edit  m_edit;
  gbact::processes::play  m_play;

public:
  void  step() noexcept override;

};


void
save_stage() noexcept
{
  auto  s = g_stage_table[g_stage_index].make_string();

#ifdef __EMSCRIPTEN__
#else
  gbstd::save_file("__stage.txt",s);
#endif
}


void
root_process::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
        if(!g_input.test_p_button())
        {
          g_object_space.show();
          g_board_view_validity.enable();

          m_chooser.initialize({
            "EDIT",
            "PLAY",
#ifndef __EMSCRIPTEN__
            "SAVE"
#endif
          },120,120);


          call(m_chooser);

          add_pc(1);
        }
      break;
  case(1):
      m_chooser.clean();

        if(get_end_value().is_integer())
        {
            switch(get_end_value().get_integer())
            {
          case(0): call(m_edit);  set_pc(2);break;
          case(1): call(m_play);  set_pc(3);break;

          case(2):
              save_stage();
              set_pc(0);
              break;
            }
        }

      else
        {
          set_pc(0);
        }
      break;
  case(2):
      m_edit.clean();

      set_pc(0);
      break;
  case(3):
      m_play.clean();

      set_pc(0);
      break;
    }
};




void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

    if(condev.dropped_file_content.size())
    {
      auto&  v = condev.dropped_file_content;

      auto&  stg = g_stage_table[g_stage_index];

      stg.build_from_string(gbstd::string_view(reinterpret_cast<const char*>(v.data()),v.size()));

      v.clear();
    }


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


      g_character_space.render(g_board_view.get_offset(),image_cursor(g_final_image,point(0,48)));
      g_object_space.render(g_board_view.get_offset(),image_cursor(g_final_image,point(0,48)));
      gbstd::g_screen_object_space.render(point(),image_cursor(g_final_image,point(0,0)));

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
                  "*\n"
                  "*立ち塞がる壁を壊しながら、彼氏のいるところへ辿り着くのが目標\n"
//                  "*上キーでジャンプ\n"
//                  "*下キーでしゃがむ\n"
                  "</pre>"
  );
#endif


  sdl::init(g_screen_width,g_screen_height,1.5);

  gbact::characters::g_image.load_from_png("__resources/lady_and.png");
  g_bg_image.load_from_png("__resources/bg.png");
  gbstd::g_misc_image.load_from_png("__resources/misc.png");

  g_final_image = sdl::make_screen_image();


  static root_process  root_prc;

  g_program.push(root_prc);

  using stage = gbact::stages::stage;

  g_board.build(g_board_width,g_board_height);

  g_board_view.set_source_image(g_bg_image);
  g_board_view.reset(g_board,g_screen_width,g_screen_height-48);


  auto&  stg = g_stage_table[g_stage_index];

  stg.build_from_string("CA101010101010101010101010100000000000000000000010100003000304000303040010102020302020202020202010100000400000000000000010100004400000040003000310103020202020202020202010104000000000000000000010104103030404040404040210101010101010101010101010");

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




