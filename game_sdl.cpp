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


namespace characters{


images::image
g_image;


class
character: public spaces::image_object
{
  int  m_phase=0;

  direction  m_direction=direction::right;

  direction  m_running_direction=direction::unknown;

  uint32_t  m_running_limit_time=0;
  uint32_t  m_last_animated_time=0;

  bool  m_ready_to_run=false;

  enum class state{
    landing,
    floating,
  } m_state=state::floating;

  enum class action{
    stand,
    walk,
    run,
    squat,
  } m_action=action::stand;

  void  move(direction  d, double  walk_value, double  run_value) noexcept
  {
    auto  ene = get_kinetic_energy();

//      if(is_landing())
      {
//          if(does_walk())
          {
              if(std::abs(ene.x) < 2)
              {
            do_walk();
                ene.x += walk_value;

                set_kinetic_energy(ene);
              }
          }
/*

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
*/
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
  image_object(g_image,rectangle(screen_width/2,0,24,48))
  {
    g_space.get_environment().set_gravitation(0.2);
    g_space.get_environment().set_fluid_kinetic_energy(real_point(0.0,0.0));
//    g_space.get_environment().set_fluid_viscosity(0.08);

    set_rendering_offset(point(-12,-48));

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

  bool  is_landing()  const noexcept{return m_state == state::landing;}
  bool  is_floating() const noexcept{return m_state == state::floating;}

  void  be_landing()  noexcept{m_state = state::landing;}
  void  be_floating() noexcept{m_state = state::floating;}

  void  do_when_collided(object&  other_side, spaces::position  position) noexcept override
  {
    auto&  area = other_side.get_area();

        if(position == spaces::position::left)
        {
          set_left(area.right);

          set_kinetic_energy_x(0);
        }

      else
        if(position == spaces::position::right)
        {
          set_right(area.left);

          set_kinetic_energy_x(0);
        }

      else
        if(position == spaces::position::top)
        {
          set_top(area.bottom);

          set_kinetic_energy_y(0);
        }

      else
        if(position == spaces::position::bottom)
        {
          set_bottom(area.top);

          set_kinetic_energy_y(0);

          be_landing();
        }
  }

  void  update() noexcept override
  {
         if(g_input.test_right_button()         ){        move(direction::right,1,2 );}
    else if(g_modified_input.test_right_button()){ready_to_run(direction::right     );}
    else if(g_input.test_left_button()          ){        move(direction::left,-1,-2);}
    else if(g_modified_input.test_left_button() ){ready_to_run(direction::left      );}
    else
      {
        do_stand();

        set_kinetic_energy_x(0);
      }


    auto  ene = get_kinetic_energy();

       if(g_input.test_down_button())
       {
           if(is_landing())
           {
             ene.x = 0;

             do_squat();
           }
       }

     else
       if(g_input.test_up_button())
       {
           if(is_landing())
           {
             ene.y -= 5;

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


     set_kinetic_energy(ene);

     object::update();

     
  }

  void  render(images::image&  dst) const noexcept override
  {
    sprite  spr;

    spr.src_image = &get_image();

    auto&  src_point = static_cast<point&>(spr.src_rectangle);

    spr.src_rectangle.w = 24;
    spr.src_rectangle.h = 48;

    spr.dst_point = get_base_point()+get_rendering_offset();

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

  static spaces::text_object  system_message("",styles::a_white_based_text_style);

    switch(get_pc())
    {
  case(0):
      system_message.set_string("PRESS ANY KEY TO START GAME");

      g_space.append_object(system_message);

      add_pc(1);
      break;
  case(1):
        if(g_input.test_all_button())
        {
          system_message.set_string("STAGE 0");

          sleep(2000);

          add_pc(1);
        }
      break;
  case(2):
      system_message.need_to_remove();

      character.set_base_point(real_point(30,120));

      g_space.append_kinetic_object(character);

      g_space.append_object(*new block_object(rectangle(     0,140,80,  16),colors::white));
      g_space.append_object(*new block_object(rectangle(     0,  0, 16,150),colors::white));
      g_space.append_object(*new block_object(rectangle(240-16,  0, 16,150),colors::white));
      g_space.append_object(*new block_object(rectangle( 80,100,32,32),colors::white));
      g_space.append_object(*new block_object(rectangle(160, 80,32,32),colors::white));
      g_space.append_object(*new block_object(rectangle(180, 60,32,32),colors::white));
      add_pc(1);

      break;
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

  program.touch(condev.time);


  static uint32_t  last;

    if(g_time >= last+60)
    {
      last = g_time;

      final_image.fill();

      g_space.render(final_image);

      sdl::update_screen(final_image);
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

  characters::g_image.load_from_png("__resources/__anigra.png");

  final_image = sdl::make_screen_image();

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




