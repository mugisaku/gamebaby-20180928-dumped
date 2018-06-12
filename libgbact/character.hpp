#ifndef character_hpp
#define character_hpp


#include"libgbstd/space.hpp"
#include"libgbstd/control_device.hpp"
#include"libgbstd/direction.hpp"
#include<new>


using namespace gbstd;


extern uint32_t  g_time;

extern keyboard  g_previous_input;
extern keyboard  g_modified_input;
extern keyboard           g_input;


extern spaces::space
g_space;


namespace gbact{
namespace characters{


extern images::image
g_image;


class player;
class bullet;

using object = spaces::object;


struct
kind_codes
{
  static constexpr uint32_t  player = 1;
  static constexpr uint32_t  bullet = 2;
  static constexpr uint32_t  lady = 3;
  static constexpr uint32_t  meat = 4;

};


class
character: public spaces::image_object
{
  direction  m_direction=direction::right;

  bool  m_visible=true;

  struct{
    bool    valid=false;
    bool  visible=false;

    uint32_t  end_time=0;

  } m_blinking_status;


  int  m_phase=0;

  uint32_t  m_last_animated_time=0;

  uint32_t  m_last_update_time=0;
  uint32_t  m_rendering_count=0;


public:
  void  reset_phase() noexcept{m_phase = 0;}
  int  get_phase() const noexcept{return m_phase;}

  bool  check_last_animated_time(uint32_t  interval) noexcept;

  bool  is_landing()  const noexcept{return get_down_contacted_square();}
  bool  is_floating() const noexcept{return !is_landing();}

  void  be_floating() noexcept{set_down_contacted_square(nullptr);}

  void       set_direction(direction  dir)       noexcept{       m_direction = dir;}
  direction  get_direction(              ) const noexcept{return m_direction      ;}

  bool  is_blinking() const noexcept{return m_blinking_status.valid;}

  void  blink(uint32_t  time) noexcept;

  void  show() noexcept{m_visible =  true;}
  void  hide() noexcept{m_visible = false;}


  virtual void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept{}
  virtual void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept{}
  virtual void  do_when_collided_with_object(object&  other_side, positions::position  position) noexcept{}


  void  do_when_collided(object&  other_side, positions::position  position) noexcept override;


  virtual void  do_when_changed_square(boards::square*  new_sq, boards::square*  old_sq) noexcept{}


  void  step(boards::board&  board) noexcept override;

  virtual bool  test_if_can_move_into_square(boards::square&  sq) const noexcept;
  virtual void  do_when_collided_with_square(boards::square&  sq) noexcept{}

  void  update_core() noexcept override;

  void  render(point  offset, images::image&  dst) noexcept override;

  static bool  m_debug;

};




class
player: public character
{
  struct{
    bool  valid=false;
    uint32_t  end_time=0;

  } m_exemption_status;


  int  m_life_level=1;

  bool  m_invincible=false;

public:
  player(int  life=1) noexcept: m_life_level(life){}

  bool  is_invincible() const noexcept{return m_invincible;}
  void    set_invincible() noexcept{m_invincible =  true;}
  void  unset_invincible() noexcept{m_invincible = false;}

  int  get_life_level() const noexcept{return m_life_level;}

  void  set_life_level(int  v) noexcept;
  void  add_life_level(int  v) noexcept;

  void  knockback() noexcept;

  void  exempt(uint32_t  time) noexcept;

  virtual void  do_when_ran_out_life() noexcept;

  bool  test_if_can_move_into_square(boards::square&  sq) const noexcept override;

  void  do_when_changed_square(boards::square*  new_sq, boards::square*  old_sq) noexcept override;

  void  update_core() noexcept override;

};




class
hero: public player
{
  enum class action{
    stand,
    walk,
    run,
    squat,
    greeting,
  } m_action=action::stand;


  direction  m_running_direction=direction::unknown;

  uint32_t  m_running_limit_time=0;
  uint32_t  m_greeting_end_time=0;

  bool  m_ready_to_run=false;

  void  ready_to_run(direction  d) noexcept;

  void  move_to_left( ) noexcept;
  void  move_to_right() noexcept;

public:
  hero() noexcept;

  bool  does_stand()    const noexcept{return m_action == action::stand;}
  bool  does_walk()     const noexcept{return m_action == action::walk;}
  bool  does_run()      const noexcept{return m_action == action::run;}
  bool  does_squat()    const noexcept{return m_action == action::squat;}
  bool  does_greeting() const noexcept{return m_action == action::greeting;}

  void  do_stand() noexcept{m_action = action::stand;}
  void  do_walk()  noexcept{m_action = action::walk;}
  void  do_run()   noexcept{m_action = action::run;}
  void  do_squat() noexcept{m_action = action::squat;}
  void  do_greeting() noexcept;


  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;

  void  update_core() noexcept override;
  void  update_graphics() noexcept override;

};


class
enemy: public player
{
  enum class action{
    sleep,
    attack,

  } m_action=action::sleep;


  uint32_t  m_time=0;

  player*  m_target=nullptr;

public:
  enemy(player*  target=nullptr) noexcept;


  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;

  void  update_core() noexcept override;
  void  update_graphics() noexcept override;

};


class
lady: public player
{
  int  m_stepping_phase=0;

  enum class action{
    stand,
    walk,
    turn,
    kick,
    orz,
    ladder,
    eat,

  } m_action=action::stand;


  void  do_when_this_is_landing() noexcept;
  void  do_when_this_is_floating() noexcept;

  void  do_when_action_is_stand() noexcept;
  void  do_when_action_is_walk() noexcept;
  void  do_when_action_is_turn() noexcept;
  void  do_when_action_is_kick() noexcept;
  void  do_when_action_is_orz() noexcept;
  void  do_when_action_is_ladder() noexcept;
  void  do_when_action_is_eat() noexcept;

public:
  lady() noexcept;

  void  update_core() noexcept override;
  void  update_graphics() noexcept override;

};


class
lady_monitor: public spaces::object
{
  lady*  m_target;

public:
  lady_monitor() noexcept{}
  lady_monitor(lady&  lady, int  x, int  y) noexcept;

  void  render(point  offset, images::image&  dst) noexcept override;

};




class
meat: public spaces::image_object
{
public:
  meat() noexcept{}
  meat(int  x, int  y) noexcept;

  void  update_graphics() noexcept override;

};




class
bullet: public character
{
  uint32_t  m_time=0;

  player*  m_shooter=nullptr;
  player*  m_target =nullptr;

public:
  bullet(player*  shooter, player*  target) noexcept;

  void      set_time(uint32_t  t)       noexcept{       m_time = t;}
  uint32_t  get_time(           ) const noexcept{return m_time    ;}

  player*  get_shooter() const noexcept{return m_shooter;}

  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_object(object&  other_side, positions::position  position) noexcept override;

  void  do_when_changed_square(boards::square*  new_sq, boards::square*  old_sq) noexcept override;
  void  do_when_collided_with_square(boards::square&  sq) noexcept override;

  void  update_core() noexcept override;
  void  update_graphics() noexcept override;

};


class
greeting_sphere: public bullet
{
public:
  greeting_sphere(player*  shooter, player*  target) noexcept;

  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_object(object&  other_side, positions::position  position) noexcept override;

  void  do_when_changed_square(boards::square*  new_sq, boards::square*  old_sq) noexcept override{}

  void  update_core() noexcept override;
  void  update_graphics() noexcept override{}

};


union
any_character
{
  character        m_character;
  player           m_player;
  hero             m_hero;
  enemy            m_enemy;
  bullet           m_bullet;
  greeting_sphere  m_greeting_sphere;

  any_character(){}
 ~any_character(){}

};


}


}




#endif




