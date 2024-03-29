#ifndef character_hpp
#define character_hpp


#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/control_device.hpp"
#include"libgbstd/direction.hpp"
#include"libgbstd/program.hpp"
#include"libgbact/board.hpp"
#include<new>


using namespace gbstd;


namespace gbact{




namespace major_ids
{
  constexpr int  player =  1;
  constexpr int  bullet =  2;
  constexpr int    item =  3;

}


namespace characters{




extern images::image
g_image;


class player;
class bullet;
class item;

using object = spaces::object;


class
physics_parameter
{
  double  m_gravitation=0;
  double  m_fluid_viscosity=0;

  real_point  m_fluid_kinetic_energy;

public:
  double  get_gravitation(         ) const noexcept{return m_gravitation    ;}
  void    set_gravitation(double  v)       noexcept{       m_gravitation = v;}

  double  get_fluid_viscosity(         ) const noexcept{return m_fluid_viscosity    ;}
  void    set_fluid_viscosity(double  v)       noexcept{       m_fluid_viscosity = v;}

  real_point  get_fluid_kinetic_energy(              ) const noexcept{return m_fluid_kinetic_energy      ;}
  void        set_fluid_kinetic_energy(real_point  pt)       noexcept{       m_fluid_kinetic_energy  = pt;}
  void        add_fluid_kinetic_energy(real_point  pt)       noexcept{       m_fluid_kinetic_energy += pt;}

};


class
physics
{
  const physics_parameter*  m_parameter=nullptr;

  bool  m_validity=false;

public:
  operator bool() const noexcept{return m_validity && m_parameter;}

  void                      set_parameter(const physics_parameter*  p)       noexcept{       m_parameter = p;}
  const physics_parameter*  get_parameter(                           ) const noexcept{return m_parameter    ;}

  bool  is_valid() const noexcept{return m_validity;}

  void   enable() noexcept{m_validity =  true;}
  void  disable() noexcept{m_validity = false;}

};


class
character: public spaces::image_object
{
  direction  m_direction=direction::right;

  struct{
    bool    valid=false;
    bool  visible=false;

    uint32_t  end_time=0;

  } m_blinking_status;


  square*            m_current_square=nullptr;
  square*     m_left_contacted_square=nullptr;
  square*    m_right_contacted_square=nullptr;
  square*      m_top_contacted_square=nullptr;
  square*   m_bottom_contacted_square=nullptr;

  physics  m_physics;

  area  m_life_area;

  point  m_square_point_offset;

  double  m_mass=0;

  int  m_phase=0;

  uint32_t  m_creation_time=0;
  uint32_t  m_life_time=0xFFFFFFFF;

  uint32_t  m_last_animated_time=0;

  uint32_t  m_last_update_time=0;

  bool  m_holding=false;

public:
  character() noexcept;

  virtual void  initialize() noexcept;

  bool  is_facing_to_left()  const noexcept{return get_direction() == direction::left;}
  bool  is_facing_to_right() const noexcept{return get_direction() == direction::right;}

  uint32_t  get_creation_time() const noexcept{return m_creation_time;}


  const area&  get_life_area(                 ) const noexcept{return m_life_area       ;}
  void         set_life_area(const area&  area)       noexcept{       m_life_area = area;}

  uint32_t  get_life_time(           ) const noexcept{return m_life_time    ;}
  void      set_life_time(uint32_t  t)       noexcept{       m_life_time = t;}

        physics&  get_physics()       noexcept{return m_physics;}
  const physics&  get_physics() const noexcept{return m_physics;}


  point  get_square_point_offset() const noexcept{return m_square_point_offset;}
  void   set_square_point_offset_x(int  v) noexcept{m_square_point_offset.x = v;}
  void   set_square_point_offset_y(int  v) noexcept{m_square_point_offset.y = v;}

  square*  get_current_square()          const noexcept{return m_current_square;}
  square*  get_left_contacted_square()   const noexcept{return m_left_contacted_square;}
  square*  get_right_contacted_square()  const noexcept{return m_right_contacted_square;}
  square*  get_top_contacted_square()    const noexcept{return m_top_contacted_square;}
  square*  get_bottom_contacted_square() const noexcept{return m_bottom_contacted_square;}


  void  reset_phase() noexcept{m_phase = 0;}
  int  get_phase() const noexcept{return m_phase;}

  bool  check_last_animated_time(uint32_t  interval) noexcept;


  void    hold() noexcept{m_holding =  true;}
  void  unhold() noexcept{m_holding = false;}

  bool  is_holding()  const noexcept{return m_holding;}


  bool  is_landing()  const noexcept{return m_bottom_contacted_square;}
  bool  is_floating() const noexcept{return !is_landing();}

  void  be_floating() noexcept{m_bottom_contacted_square = nullptr;}

  void       set_direction(direction  dir)       noexcept{       m_direction = dir;}
  direction  get_direction(              ) const noexcept{return m_direction      ;}

  bool  is_blinking() const noexcept{return m_blinking_status.valid;}

  void  blink(uint32_t  time) noexcept;

  void  block(character&  target, positions::position  position) const noexcept;


  virtual void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept{}
  virtual void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept{}
  virtual void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept{}

  virtual void  do_when_collided(character&  other_side, positions::position  position) noexcept{}


  virtual void  do_when_changed_square(square*  new_sq, square*  old_sq) noexcept{}


  void  detect_current_square() noexcept;

  virtual void  do_when_collided_with_square(square&  sq) noexcept{}

  void  update_core() noexcept override;

  void  render(point  offset, image_cursor  cur) noexcept override;

  virtual void  render_additionally(point  offset, image_cursor  cur) noexcept{}

  static bool  m_debug;

};




namespace player_ids{
constexpr uint32_t  unknown = 0;
constexpr uint32_t     lady = 1;
constexpr uint32_t      boy = 2;
constexpr uint32_t     wall = 3;
}


class
player: public character
{
  int  m_life_level=1;

  int  m_blocking_bits=0;

public:
  player(int  life=1) noexcept;

  int   get_blocking_bits(      ) const noexcept{return m_blocking_bits    ;}
  void  set_blocking_bits(int  v)       noexcept{       m_blocking_bits = v;}

  int  get_life_level() const noexcept{return m_life_level;}

  void  set_life_level(int  v) noexcept;
  void  add_life_level(int  v) noexcept;

  void  knockback() noexcept;


  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept override;

  void  do_when_collided(character&  other_side, positions::position  position) noexcept override;


  virtual void  do_when_ran_out_life() noexcept;

  void  do_when_changed_square(square*  new_sq, square*  old_sq) noexcept override;

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
    meet,

  } m_action=action::stand;


  enum class state{
    challenging,
    rejoicing,
    crying,
  } m_state=state::challenging;


  void  do_when_this_is_landing() noexcept;
  void  do_when_this_is_floating() noexcept;

  void  do_when_action_is_stand() noexcept;
  void  do_when_action_is_walk() noexcept;
  void  do_when_action_is_turn() noexcept;
  void  do_when_action_is_kick() noexcept;
  void  do_when_action_is_orz() noexcept;
  void  do_when_action_is_ladder() noexcept;
  void  do_when_action_is_eat() noexcept;
  void  do_when_action_is_meet() noexcept;

public:
  lady() noexcept;

  bool  is_challenging() const noexcept{return m_state == state::challenging;}
  bool  is_rejoicing()   const noexcept{return m_state == state::rejoicing;}
  bool  is_crying()      const noexcept{return m_state == state::crying;}

  void  reset() noexcept;

  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept override;

  void  do_when_collided_with_square(square&  sq) noexcept override;

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

  void  render(point  offset, image_cursor  cur) noexcept override;

};




class
boy: public player
{
public:
  boy() noexcept;

  void  reset() noexcept;

  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept override;

};


class
wall: public player
{
public:
  wall( int  x=0, int  y=0) noexcept;

  static int  get_count() noexcept;

  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept override;

};




class
item: public character
{
public:
  item() noexcept;

  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept override;

  void  do_when_collided(character&  other_side, positions::position  position) noexcept override;

};




class
meat: public item
{
public:
  meat() noexcept;

};




namespace bullet_ids{
constexpr int  kick = 1;
}


class
bullet: public character
{
public:
  using callback = void(*)(characters::bullet&  bullet, character&  other_side);

private:
  int  m_x_distance=0;
  int  m_y_distance=0;

  character*  m_shooter=nullptr;
  character*   m_target=nullptr;

  struct flags{
    static constexpr int          heat = 0x0001;
    static constexpr int           ice = 0x0002;
    static constexpr int         shock = 0x0004;
    static constexpr int         shake = 0x0008;
    static constexpr int   penetrating = 0x0010;
    static constexpr int   electricity = 0x0020;
    static constexpr int         water = 0x0040;
    static constexpr int          dust = 0x0080;
    static constexpr int         light = 0x0100;
    static constexpr int      chemical = 0x0200;
    static constexpr int       presure = 0x0400;
    static constexpr int       cutting = 0x0800;
    static constexpr int     radiowave = 0x1000;

  };

  int  m_attribute=0;

  int  m_destructive_power=0;

  callback  m_callback=nullptr;

public:
  bullet(character*  shooter, character*  target) noexcept;

  void  set_dust_flag() noexcept{m_attribute |= flags::dust;}

  void  set_destructive_power(int  v)       noexcept{       m_destructive_power = v;}
  int   get_destructive_power(      ) const noexcept{return m_destructive_power    ;}

  character*  get_shooter() const noexcept{return m_shooter;}

  void  do_when_collided_with_bullet(bullet&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;
  void  do_when_collided_with_item(    item&  other_side, positions::position  position) noexcept override;

  void  do_when_collided(character&  other_side, positions::position  position) noexcept override;

  void  do_when_changed_square(square*  new_sq, square*  old_sq) noexcept override;
  void  do_when_collided_with_square(square&  sq) noexcept override;

  void  set_callback(callback  cb) noexcept{m_callback = cb;}

  void  hit(character&  other_side) noexcept;

};


class
greeting_sphere: public bullet
{
public:
  greeting_sphere(player*  shooter, player*  target) noexcept;

  void  do_when_collided_with_player(player&  other_side, positions::position  position) noexcept override;

  void  do_when_changed_square(square*  new_sq, square*  old_sq) noexcept override{}

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


struct
character_set
{
  characters::lady  m_lady;
  characters::boy   m_boy;

  std::vector<characters::meat>  m_meats;
  std::vector<characters::wall>  m_walls;

};


using characters::character;


}


extern spaces::space<gbact::character>
g_character_space;


extern spaces::space<spaces::object>
g_object_space;


extern spaces::space<spaces::object>
g_screen_object_space;




#endif




