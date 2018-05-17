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

  real_point  m_point;
  real_point  m_vector;

  int  m_phase=0;

  direction  m_direction=direction::right;

  uint32_t  m_last_update_time=0;

  enum class action{
    stand,
    walk,
    squat,
  } m_action=action::stand;

  enum class state{
    landing,
    floating,

  } m_state=state::floating;

public:
  character() noexcept
  {
    m_point.x = screen_width/2;

    m_environment.m_gravitation = 0.2;
  }

  void  update(const control_device&  ctrl) noexcept
  {
    auto&  kbd = ctrl.keyboard;

       if(kbd.right_button)
       {
           if(m_state == state::landing)
           {
             m_vector.x = 1;

             m_action = action::walk;
           }


         m_direction = direction::right;
       }

     else
       if(kbd.left_button)
       {
           if(m_state == state::landing)
           {
             m_vector.x = -1;

             m_action = action::walk;
           }


         m_direction = direction::left;
       }

     else
       {
         m_action = action::stand;
       }


       if(kbd.down_button)
       {
           if(m_state == state::landing)
           {
             m_vector.x = 0;

             m_action = action::squat;
           }
       }

     else
       if(kbd.up_button)
       {
           if(m_state == state::landing)
           {
             m_vector.y -= 4;

             m_action = action::stand;

             m_state = state::floating;
           }
       }


       if(ctrl.time >= (m_last_update_time+160))
       {
         m_last_update_time = ctrl.time;

           if(++m_phase > 3)
           {
             m_phase = 0;
           }
       }


       if(m_state == state::floating)
       {
         m_vector.y += m_environment.m_gravitation;
       }


     m_point += m_vector;

       if(m_state == state::landing)
       {
         m_vector.x /= 2;
       }


       if(m_point.y > screen_height)
       {
         m_point.y = screen_height;

         m_state = state::landing;

         m_vector.y = 0;
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
        spr.src_point = point( 0,0);
        break;
    case(action::walk):
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
  void  step(const control_device&  ctrl) noexcept override;

} ctx;


void
player_context::
step(const control_device&  ctrl) noexcept
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

  stack.touch(condev);

  garden.clear();

  character.update(condev);

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
  set_caption("mkanigra - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*PNGファイルをドラッグ・アンド・ドロップで読み込む"
                  "</pre>"
  );
#endif


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
  sdl::init(screen_width,screen_height);

  characters::image.load_from_png("../bin/__anigra.png");

  final_image = sdl::make_screen_image();

  stack.push(ctx);

    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




