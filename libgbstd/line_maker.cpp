#include"libgbstd/line_maker.hpp"
#include<algorithm>



namespace gbstd{


line_maker::
line_maker(line  ln) noexcept
{
  constexpr int  base = (1<<shift_amount)/2;

  auto  const x_min = std::min(ln.a_point.x,ln.b_point.x);
  auto  const x_max = std::max(ln.a_point.x,ln.b_point.x);
  auto  const y_min = std::min(ln.a_point.y,ln.b_point.y);
  auto  const y_max = std::max(ln.a_point.y,ln.b_point.y);

  int  x_distance = x_max-x_min;
  int  y_distance = y_max-y_min;

  m_point.x = base+(ln.a_point.x<<shift_amount);
  m_point.y = base+(ln.a_point.y<<shift_amount);

    if(!x_distance)
    {
      m_x_add_amount = (0              );
      m_y_add_amount = (1<<shift_amount);

      m_distance = y_distance;
    }

  else
    if(!y_distance)
    {
      m_x_add_amount = (1<<shift_amount);
      m_y_add_amount = (0              );

      m_distance = x_distance;
    }

  else
    if(x_distance < y_distance)
    {
      m_x_add_amount = (x_distance<<shift_amount)/y_distance;
      m_y_add_amount = (         1<<shift_amount);

      m_distance = y_distance;
    }

  else
    if(x_distance > y_distance)
    {
      m_x_add_amount = (         1<<shift_amount);
      m_y_add_amount = (y_distance<<shift_amount)/x_distance;

      m_distance = x_distance;
    }

  else
    {
      m_x_add_amount = (1<<shift_amount);
      m_y_add_amount = (1<<shift_amount);

      m_distance = x_distance;
    }


    if(ln.a_point.x > ln.b_point.x){m_x_add_amount *= -1;}
    if(ln.a_point.y > ln.b_point.y){m_y_add_amount *= -1;}
}




point
line_maker::
get_point() const noexcept
{
  return point(m_point.x>>shift_amount,
               m_point.y>>shift_amount);
}


void
line_maker::
step() noexcept
{
  m_point.x += m_x_add_amount;
  m_point.y += m_y_add_amount;

  --m_distance;
}


}





