#ifndef gbwar_process_HPP
#define gbwar_process_HPP


#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"


using namespace gbstd;


namespace processes{


class
move: public programs::process
{
  spaces::object*  m_target=nullptr;

  int  m_x_add_amount;
  int  m_y_add_amount;

  int  m_count;

public:
  void  reset(spaces::object&  t, int  x, int  y) noexcept
  {
    m_target = &t;

    m_x_add_amount = x;
    m_y_add_amount = y;

    m_count = 0;
  }

  void  step() noexcept override
  {
    m_target->add_base_point(m_x_add_amount,m_y_add_amount);

      if(m_count >= square::size)
      {
        end();
      }
  }

};


}




#endif




