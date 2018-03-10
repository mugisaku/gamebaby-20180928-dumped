#ifndef GMBB_LineMaker_HPP_INCLUDED
#define GMBB_LineMaker_HPP_INCLUDED


#include"image.hpp"




namespace gbstd{


class
line_maker
{
  static constexpr int  shift_amount = 16;

  int  m_distance=0;

  int  m_x_add_amount=0;
  int  m_y_add_amount=0;

  int  m_x;
  int  m_y;

public:
  line_maker(int  x0=0, int  y0=0, int  x1=0, int  y1=0) noexcept;

  int  get_distance() const noexcept{return m_distance;}

  int  get_x() const noexcept{return m_x>>shift_amount;}
  int  get_y() const noexcept{return m_y>>shift_amount;}

  void  step() noexcept;

};


}




#endif




