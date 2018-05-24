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


class
character: public spaces::image_object
{
protected:
  int  m_phase=0;

  uint32_t  m_last_animated_time=0;

  direction  m_direction=direction::right;

  enum class state{
    landing,
    floating,
  } m_state=state::floating;

public:
  character() noexcept;

  bool  is_landing()  const noexcept{return m_state == state::landing;}
  bool  is_floating() const noexcept{return m_state == state::floating;}

  void  be_landing()  noexcept{m_state = state::landing;}
  void  be_floating() noexcept{m_state = state::floating;}

  void  do_when_collided(object&  other_side, spaces::position  position) noexcept override;

  void  render(images::image&  dst) const noexcept override;

};


class
player: public character
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

  void  update() noexcept override;

  void  render(images::image&  dst) const noexcept;

};


class
enemy: public character
{
};




}}




#endif




