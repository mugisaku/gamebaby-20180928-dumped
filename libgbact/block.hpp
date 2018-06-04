#ifndef block_hpp
#define block_hpp


#include"libgbstd/board.hpp"
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
namespace blocks{


struct
kind_codes
{
  static constexpr uint32_t      brick = 1;
  static constexpr uint32_t  waterfall = 2;

};


class
block: public boards::square_data
{
  bool  m_wall_flag=false;

public:
  bool  test_wall_flag() const noexcept{return m_wall_flag;}

  void    set_wall_flag() noexcept{m_wall_flag =  true;}
  void  unset_wall_flag() noexcept{m_wall_flag = false;}

};


class
brick: public block
{
public:
  brick() noexcept{
    set_wall_flag();
    set_image_point(point(24,0));
  }

};


class
waterfall: public block
{
  int  m_phase=0;

public:
  waterfall() noexcept{
    set_image_point(point(48,0));
  }

  void  step() noexcept
  {
      if(++m_phase >= 3)
      {
        m_phase = 0;
      }


    set_image_point(point(48+(24*m_phase),0));
  }

};


}


}




#endif




