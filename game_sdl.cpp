#include"libgbstd/context.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/direction.hpp"
#include"sdl.hpp"


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


namespace characters{


images::image
image;


struct
environment
{
  double  m_gravitation;//重力

  double  m_fluid_viscosity;//流体粘土

  real_point  m_fluid_vector;//流体方向

};


class
character
{
  environment  m_environment;

  real_point  m_previous_point;
  real_point  m_point;
  real_point  m_vector;

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

  enum class state{
    landing,
    floating,

  } m_state=state::floating;

  void  move(direction  d, double  walk_value, double  run_value) noexcept
  {
      if(is_landing())
      {
          if(does_walk())
          {
            m_vector.x = walk_value;
          }

        else
          if(does_run())
          {
            m_vector.x = run_value;
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
  character() noexcept
  {
    m_point.x = screen_width/2;

    m_environment.m_gravitation = 0.2;
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

  void  update() noexcept
  {
          if(g_input.test_right_button()         ){        move(direction::right,2,4 );}
     else if(g_modified_input.test_right_button()){ready_to_run(direction::right     );}
     else if(g_input.test_left_button()          ){        move(direction::left,-2,-4);}
     else if(g_modified_input.test_left_button() ){ready_to_run(direction::left      );}
     else {do_stand();}


       if(g_input.test_down_button())
       {
           if(is_landing())
           {
             m_vector.x = 0;

             do_squat();
           }
       }

     else
       if(g_input.test_up_button())
       {
           if(is_landing())
           {
             m_vector.y -= 4;

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


       if(is_floating())
       {
         m_vector.y += m_environment.m_gravitation;
       }


     m_previous_point = m_point            ;
                        m_point += m_vector;

       if(is_landing())
       {
         m_vector.x /= 2;
       }


       if(m_previous_point.x > m_point.x)
       {
           if(m_point.x < 0)
           {
             m_point.x = 0;

             m_vector.x = 0;
           }
       }

     else
       if(m_previous_point.x < m_point.x)
       {
           if(m_point.x > screen_width)
           {
             m_point.x = screen_width;

             m_vector.x = 0;
           }
       }


       if(m_previous_point.y < m_point.y)
       {
           if(m_point.y > (screen_height/2))
           {
             m_point.y = (screen_height/2);

             m_state = state::landing;

             m_vector.y = 0;
           }
       }
  }

  sprite  make_sprite() const noexcept
  {
    sprite  spr;

    spr.image = &image;

    spr.width  = 24;
    spr.height = 48;

      switch(m_action)
      {
    case(action::stand):
        spr.src_point = (m_state == state::landing)? point( 0,0)
                       :                             point(24,0);
        break;
    case(action::walk):
    case(action::run):
        spr.src_point = (m_phase == 0)? point( 0,0)
                       :(m_phase == 1)? point(24,0)
                       :(m_phase == 2)? point( 0,0)
                       :                point(48,0);
        break;
    case(action::squat):
        spr.src_point = point(24*3,0);
        break;
      }


      if(m_direction == direction::left)
      {
        spr.width = -spr.width;
      }


    spr.dst_point = m_point-point(24/2,48);

    return spr;
  }

};


}




namespace{


characters::character
character;


images::image
final_image;


std::vector<sprite>
garden;


contexts::context_stack
stack;


class
player_context: public context
{
public:
  void  step() noexcept override;

} ctx;


void
player_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      add_pc(1);
      break;
  case(1):
      
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

  stack.touch(condev.time);

  garden.clear();

  character.update();

  garden.emplace_back(character.make_sprite());

  final_image.fill();

    for(auto&  s: garden)
    {
      s.render(final_image);
    }


  sdl::update_screen(final_image);
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("game - " __DATE__);
  set_description("<pre>"
                  "*キーボードで操作\n"
                  "*左右で移動、上でジャンプ、下でしゃがむ\n"
                  "</pre>"
  );
#endif


  sdl::init(screen_width,screen_height);

  characters::image.load_from_png("__resources/__anigra.png");

  final_image = sdl::make_screen_image();

  stack.push(ctx);


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




