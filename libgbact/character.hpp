#ifndef character_hpp
#define character_hpp


#include"libgbstd/space.hpp"
#include"libgbstd/control_device.hpp"
#include"libgbstd/direction.hpp"


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


class character;


class
character_data
{
protected:
  int  m_phase=0;

  uint32_t  m_last_animated_time=0;

  direction  m_direction=direction::right;

  character*  m_shooter=nullptr;

  enum class state{
    landing,
    floating,
  } m_state=state::floating;


  character*  m_character=nullptr;

public:
  void        set_character(character&  chr)       noexcept{        m_character = &chr;}
  character&  get_character(               ) const noexcept{return *m_character       ;}


  bool  is_landing()  const noexcept{return m_state == state::landing;}
  bool  is_floating() const noexcept{return m_state == state::floating;}

  void  be_landing()  noexcept{m_state = state::landing;}
  void  be_floating() noexcept{m_state = state::floating;}

  void       set_direction(direction  dir)       noexcept{       m_direction = dir;}
  direction  get_direction(              ) const noexcept{return m_direction      ;}


  virtual void  initialize_character() noexcept{}

  virtual void  do_when_collided_with_character(character&  other_side, spaces::position  position) noexcept;

  virtual void  do_when_collided_with_object(spaces::object&  other_side, spaces::position  position) noexcept;

  virtual void  update_image() noexcept{}
  virtual void  update_character() noexcept{}

};


class
character: public spaces::image_object
{
  character_data*  m_data=nullptr;

public:
  void             set_data(character_data*  dat)       noexcept;
  character_data*  get_data(                    ) const noexcept{return m_data;}

  const char*  get_class_id() const noexcept override;

  bool  query(uint32_t  code) const noexcept override{return true;}

  void  do_when_collided(object&  other_side, spaces::position  position) noexcept override;

  void  update() noexcept override;

  void  render(images::image&  dst) const noexcept override;

};


class
player: public character_data
{
  enum class action{
    stand,
    walk,
    run,
    squat,
  } m_action=action::stand;


  direction  m_running_direction=direction::unknown;

  uint32_t  m_running_limit_time=0;

  bool  m_ready_to_run=false;

  void  ready_to_run(direction  d) noexcept;

  void  move(direction  d, double  walk_value, double  run_value) noexcept;

public:
  player() noexcept;

  bool  does_stand()  const noexcept{return m_action == action::stand;}
  bool  does_walk()   const noexcept{return m_action == action::walk;}
  bool  does_run()    const noexcept{return m_action == action::run;}
  bool  does_squat()  const noexcept{return m_action == action::squat;}

  void  do_stand() noexcept{m_action = action::stand;}
  void  do_walk()  noexcept{m_action = action::walk;}
  void  do_run()   noexcept{m_action = action::run;}
  void  do_squat() noexcept{m_action = action::squat;}


  void  initialize_character() noexcept override;

  void  update_character() noexcept override;
  void  update_image() noexcept override;

};




}}




#endif




