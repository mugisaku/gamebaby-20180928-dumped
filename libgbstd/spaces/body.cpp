#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
body::
set_base_point(real_point  pt) noexcept
{
  m_base_point = pt;

  update();
}


void
body::
add_base_point(real_point  pt) noexcept
{
  m_base_point += pt;

  update();
}


point
body::
get_point() const noexcept
{
  return point(static_cast<int>(m_base_point.x)+m_offset.x,
               static_cast<int>(m_base_point.y)+m_offset.y);
}


void
body::
set_left_position(int  v) noexcept
{
  m_left_position  = v        ;
  m_right_position = v+m_width;

  m_base_point.x = v-m_offset.x;
}


void
body::
set_right_position(int  v) noexcept
{
  m_right_position = v        ;
  m_left_position  = v-m_width;

  m_base_point.x = m_left_position-m_offset.x;
}


void
body::
set_top_position(int  v) noexcept
{
  m_top_position    = v         ;
  m_bottom_position = v+m_height;

  m_base_point.y = v-m_offset.y;
}


void
body::
set_bottom_position(int  v) noexcept
{
  m_bottom_position = v         ;
  m_top_position    = v-m_height;

  m_base_point.y = m_top_position-m_offset.y;
}




void
body::
update() noexcept
{
  auto  pt = get_point();

  m_left_position = pt.x;
  m_top_position  = pt.y;

  m_right_position  = m_left_position+m_width ;
  m_bottom_position = m_top_position +m_height;
}


rectangle
body::
get_rectangle() const noexcept
{
  rectangle  rect(get_point(),m_width,m_height);

  return rect;
}




void
body::
print() const noexcept
{
  printf("base_point: {%8f, %8f}\n",m_base_point.x,m_base_point.y);
}




bool
body::
test_x_collision(const body&  a, const body&  b) noexcept
{
    if(a.m_left_position <= b.m_left_position){return a.m_right_position >= b.m_left_position;}
  else                                        {return b.m_right_position >= a.m_left_position;}
}


bool
body::
test_y_collision(const body&  a, const body&  b) noexcept
{
    if(a.m_top_position <= b.m_top_position){return a.m_bottom_position >= b.m_top_position;}
  else                                      {return b.m_bottom_position >= a.m_top_position;}
}


bool
body::
test_collision(const body&  a, const body&  b) noexcept
{
  return(test_x_collision(a,b) &&
         test_y_collision(a,b));
}


}}




