#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




/*
object&
object::
assign(object&&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_name = std::move(rhs.m_name);

      m_base_point      = rhs.m_base_point;
      m_last_base_point = rhs.m_last_base_point;

      m_offset = rhs.m_offset;

      m_width  = rhs.m_width;
      m_height = rhs.m_height;

      m_kinetic_energy = rhs.m_kinetic_energy;

      m_area = rhs.m_area;
      m_last_area = rhs.m_last_area;

      std::swap(m_dieing_counter,rhs.m_dieing_counter);

      m_rendering_counter = rhs.m_rendering_counter;

      m_state =  = rhs.m_stte;
    }


  return *this;
}


object&
object::
assign(const object&  rhs) noexcept
{
    if(this != &rhs)
    {
      die();

      m_name = rhs.m_name;

      m_base_point      = rhs.m_base_point;
      m_last_base_point = rhs.m_last_base_point;

      m_offset = rhs.m_offset;

      m_width  = rhs.m_width;
      m_height = rhs.m_height;

      m_kinetic_energy = rhs.m_kinetic_energy;

      m_area = rhs.m_area;
      m_last_area = rhs.m_last_area;

      m_dieing_counter,rhs.m_dieing_counter);

      m_rendering_counter = rhs.m_rendering_counter;

      m_state =  = rhs.m_stte;
    }


  return *this;
}
*/




void
object::
be_alive(uint32_t&  v) noexcept
{
    if(!m_dieing_counter)
    {
      m_dieing_counter = &v;
    }
}


void
object::
die() noexcept
{
    if(m_dieing_counter)
    {
      (*m_dieing_counter) += 1;

      m_dieing_counter = nullptr;
    }
}


void
object::
update_base_point() noexcept
{
  m_last_area = m_area;

  m_last_base_point = m_base_point                    ;
                      m_base_point += m_kinetic_energy;

  update_area();
}


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
print() const noexcept
{
  printf("body:{\n");
  printf("    base_point: {%8f, %8f}\n",m_base_point.x,m_base_point.y);
  printf("kinetic energy: {%8f, %8f}\n",m_kinetic_energy.x,m_kinetic_energy.y);

  m_area.print();

  printf("}\n");
}




}}




