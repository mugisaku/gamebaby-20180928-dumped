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
set_left(int  v) noexcept
{
  m_area.left  = v          ;
  m_area.right = v+m_width-1;

  m_base_point.x = v-m_offset.x;
}


void
body::
set_right(int  v) noexcept
{
  m_area.right = v          ;
  m_area.left  = v-m_width+1;

  m_base_point.x = m_area.left-m_offset.x;
}


void
body::
set_top(int  v) noexcept
{
  m_area.top    = v           ;
  m_area.bottom = v+m_height-1;

  m_base_point.y = v-m_offset.y;
}


void
body::
set_bottom(int  v) noexcept
{
  m_area.bottom = v           ;
  m_area.top    = v-m_height+1;

  m_base_point.y = m_area.top-m_offset.y;
}




void
body::
update() noexcept
{
  auto  pt = get_point();

  m_area.left = pt.x;
  m_area.top  = pt.y;

  m_area.right  = m_area.left+m_width -1;
  m_area.bottom = m_area.top +m_height-1;
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
  printf("body:{\n");
  printf("  base_point: {%8f, %8f}\n",m_base_point.x,m_base_point.y);
  printf("}\n");
}




}}




