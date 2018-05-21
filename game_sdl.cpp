#include"libgbstd/program.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/direction.hpp"
#include"sdl.hpp"
#include<cmath>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


constexpr int  screen_width  = 240;
constexpr int  screen_height = 240;


uint32_t  g_time = 0;

keyboard  g_previous_input;
keyboard  g_modified_input;
keyboard           g_input;


images::image
title_image;


spaces::space
g_space;


namespace characters{


images::image
image;


class
character: public spaces::object
{
  const images::image*  m_image=nullptr;

  rectangle  m_image_rectangle;

  point  m_rendering_offset;

  int  m_phase=0;

  direction  m_direction=direction::right;

  direction  m_running_direction=direction::unknown;

  uint32_t  m_running_limit_time=0;
  uint32_t  m_last_animated_time=0;

  bool  m_ready_to_run=false;

  enum class action{
    stand,
    walk,
    run,
    squat,
  } m_action=action::stand;

  void  move(direction  d, double  walk_value, double  run_value) noexcept
  {
add_base_point(real_point(walk_value,0));
return;
      if(is_landing())
      {
          if(does_walk())
          {
              if(std::abs(m_kinetic_energy.x) < 2)
              {
//                m_kinetic_energy.x += walk_value;
              }
          }

        else
          if(does_run())
          {
              if(std::abs(m_kinetic_energy.x) < 4)
              {
                m_kinetic_energy.x += run_value;
              }
          }

        else
          if(m_ready_to_run)
          {
              if((g_time < m_running_limit_time) &&
                 (m_running_direction == d))
              {
                do_run();
              }

            else
              {
                m_ready_to_run = false;
              }
          }

        else
          {
            do_walk();
          }
       }


    m_direction = d;
  }

  void  ready_to_run(direction  d) noexcept
  {
      if(is_landing() && !m_ready_to_run)
      {
        m_running_direction = d;

        m_running_limit_time = g_time+200;

        m_ready_to_run = true;
      }
  }

public:
  character() noexcept:
  object(rectangle(screen_width/2,0,24,48))
  {
    g_space.get_environment().set_gravitation(0.2);
    g_space.get_environment().set_fluid_kinetic_energy(real_point(0.0,0.0));
//    g_space.get_environment().set_fluid_viscosity(0.08);

    m_rendering_offset = point(-12,-48);

    set_offset(point(-12,-48));
  }

  bool  does_stand()  const noexcept{return m_action == action::stand;}
  bool  does_walk()   const noexcept{return m_action == action::walk;}
  bool  does_run()    const noexcept{return m_action == action::run;}
  bool  does_squat()  const noexcept{return m_action == action::squat;}

  void  do_stand() noexcept{m_action = action::stand;}
  void  do_walk()  noexcept{m_action = action::walk;}
  void  do_run()   noexcept{m_action = action::run;}
  void  do_squat() noexcept{m_action = action::squat;}

  void  update() noexcept override
  {
          if(g_input.test_right_button()         ){        move(direction::right,1,0.02 );}
     else if(g_modified_input.test_right_button()){ready_to_run(direction::right     );}
     else if(g_input.test_left_button()          ){        move(direction::left,-1,-0.2);}
     else if(g_modified_input.test_left_button() ){ready_to_run(direction::left      );}
     else {do_stand();}


       if(g_input.test_down_button())
       {
add_base_point(real_point(0,1));
           if(is_landing())
           {
//             m_kinetic_energy.x = 0;

             do_squat();
           }
       }

     else
       if(g_input.test_up_button())
       {
add_base_point(real_point(0,-1));
           if(is_landing())
           {
//             m_kinetic_energy.y -= 5;

             do_stand();

             be_floating();
           }
       }


       if(g_time >= (m_last_animated_time+160))
       {
         m_last_animated_time = g_time;

           if(++m_phase > 3)
           {
             m_phase = 0;
           }
       }


     object::update();
  }

  void  render(images::image&  dst) const noexcept override
  {
    sprite  spr;

    spr.src_image = &characters::image;

    auto&  src_point = static_cast<point&>(spr.src_rectangle);

    spr.src_rectangle.w = 24;
    spr.src_rectangle.h = 48;

    spr.dst_point = get_base_point()+m_rendering_offset;

      switch(m_action)
      {
    case(action::stand):
        src_point = (m_state == state::landing)? point( 0,0)
                   :                             point(24,0);
        break;
    case(action::walk):
    case(action::run):
        src_point = (m_phase == 0)? point( 0,0)
                   :(m_phase == 1)? point(24,0)
                   :(m_phase == 2)? point( 0,0)
                   :                point(48,0);
        break;
    case(action::squat):
        src_point = point(24*3,0);
        break;
      }


      if(m_direction == direction::left)
      {
        spr.src_rectangle.w = -spr.src_rectangle.w;
      }


    spr.render(dst);


    auto  rect = get_rectangle();

    dst.draw_rectangle_safely(colors::red,rect.x,rect.y,rect.w,rect.h);
  }

};


}




namespace{


characters::character
character;


images::image
final_image;


programs::program
program;


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

    switch(get_pc())
    {
  case(0):
      final_image.fill();

      g_space.update();

      g_space.render(final_image);

      sdl::update_screen(final_image);

      add_pc(1);
      break;
  case(1):
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
              final_image.fill();

              g_space.update();

              g_space.detect_ds_collision(spaces::default_detection);

              g_space.render(final_image);

              sdl::update_screen(final_image);
            }
        }
      break;
  case(2):
      add_pc(-1);
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

  program.touch(condev.time);
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("game - " __DATE__);
  set_description("<pre>"
                  "*キーボードで操作\n"
                  "*左右キーを押すと歩く。二回素早く押すと走る\n"
                  "*上キーでジャンプ\n"
                  "*下キーでしゃがむ\n"
                  "*目的などは、まだ無い"
                  "</pre>"
  );
#endif


  sdl::init(screen_width,screen_height);

  characters::image.load_from_png("__resources/__anigra.png");
  title_image.load_from_png("__resources/title_ch.png");

  final_image = sdl::make_screen_image();

/*
  auto  o = new spaces::rectangle_object(rectangle(     0,  0, 16,150),colors::white);

  o->be_fixed();

  o->set_mark(1);
*/
  character.set_base_point(real_point(30,120));

  g_space.append_dynamical_object(character);
/*
  g_space.append_statical_object((new spaces::rectangle_object(rectangle(     0,150,240, 16),colors::white))->be_fixed());
  g_space.append_statical_object(*o);
  g_space.append_statical_object((new spaces::rectangle_object(rectangle(240-16,  0, 16,150),colors::white))->be_fixed());
*/
  g_space.append_statical_object((new spaces::rectangle_object(rectangle(64,80,32,32),colors::white))->be_fixed());

  program.push(ctx);


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




