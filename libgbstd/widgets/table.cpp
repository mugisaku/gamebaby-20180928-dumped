#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
table_column::
reform(point  abs_pt) noexcept
{
  int  y = 0;
  int  w = 1;
  int  h = 1;

  auto  current = m_first_child;

    while(current)
    {
      point  rel_pt(0,y);

      current->set_relative_point(rel_pt);

      current->reform(abs_pt+rel_pt);

      current->unset_flag(flags::needed_to_reform);


      y += current->get_height();

      w = std::max(w,rel_pt.x+current->get_width() );
      h = std::max(h,rel_pt.y+current->get_height());

      current = current->get_next();
    }


  m_width  = w;
  m_height = h;

  widget::reform(abs_pt);
}


void
table_column::
append(std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      container::append_child(w,0,0);
    }
}




void
table_row::
reform(point  abs_pt) noexcept
{
  int  x = 0;
  int  w = 1;
  int  h = 1;

  auto  current = m_first_child;

    while(current)
    {
      point  rel_pt(x,0);

      current->set_relative_point(rel_pt);

      current->reform(abs_pt+rel_pt);

      current->unset_flag(flags::needed_to_reform);


      x += current->get_width();


      w = std::max(w,rel_pt.x+current->get_width() );
      h = std::max(h,rel_pt.y+current->get_height());

      current = current->get_next();
    }


  m_width  = w;
  m_height = h;

  widget::reform(abs_pt);
}


void
table_row::
append(std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      container::append_child(w,0,0);
    }
}




}}




