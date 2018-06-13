#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




point
object::
get_point() const noexcept
{
  return point(static_cast<int>(m_base_point.x)+m_offset.x,
               static_cast<int>(m_base_point.y)+m_offset.y);
}


void
object::
set_left(int  v) noexcept
{
  m_area.left  = v            ;
  m_area.right = v+(m_width-1);

  m_base_point.x = v-m_offset.x;
}


void
object::
set_right(int  v) noexcept
{
  m_area.right = v            ;
  m_area.left  = v-(m_width-1);

  m_base_point.x = m_area.left-m_offset.x;
}


void
object::
set_top(int  v) noexcept
{
  m_area.top    = v             ;
  m_area.bottom = v+(m_height-1);

  m_base_point.y = v-m_offset.y;
}


void
object::
set_bottom(int  v) noexcept
{
  m_area.bottom = v             ;
  m_area.top    = v-(m_height-1);

  m_base_point.y = m_area.top-m_offset.y;
}




void
object::
update_area() noexcept
{
  auto  pt = get_point();

  m_area.left = pt.x;
  m_area.top  = pt.y;

  m_area.right  = m_area.left+m_width -1;
  m_area.bottom = m_area.top +m_height-1;
}


rectangle
object::
get_rectangle() const noexcept
{
  rectangle  rect(get_point(),m_width,m_height);

  return rect;
}




void
object::
update_core() noexcept
{
  save_area();


  auto  ene = get_kinetic_energy();

  add_base_point(ene);

  update_area();
}


void
object::
print() const noexcept
{
  printf("body:{\n");
  printf("    base_point: {%8f, %8f}\n",m_base_point.x,m_base_point.y);
  printf("kinetic energy: {%8f, %8f}\n",m_kinetic_energy.x,m_kinetic_energy.y);

  m_area.print();

  printf("}\n");
}




}}




