#ifndef GMBB_LineMaker_HPP_INCLUDED
#define GMBB_LineMaker_HPP_INCLUDED


#include"image.hpp"




namespace gbstd{


class
line_maker
{
  static constexpr int  shift_amount = 16;

  point  m_point;

  int  m_distance;

  int  m_x_add_amount;
  int  m_y_add_amount;

public:
  line_maker(line  ln) noexcept;

  int  get_distance() const noexcept{return m_distance;}

  point  get_point() const noexcept;

  void  step() noexcept;

};


}




#endif




