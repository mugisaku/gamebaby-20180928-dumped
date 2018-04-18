#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
table_column::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

  int  y = 0;
  int  w = 1;
  int  h = 1;

    for(auto&  child: m_children)
    {
      point  rel_pt(0,y);

      child->set_relative_point(rel_pt);

      child->reform_if_needed(get_absolute_point());


      y += child->get_height();

      w = std::max(w,rel_pt.x+child->get_width() );
      h = std::max(h,rel_pt.y+child->get_height());
    }


  m_width  = w;
  m_height = h;
}


void
table_column::
append_child(std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      container::append_child(w);
    }
}




void
table_row::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

  int  x = 0;
  int  w = 1;
  int  h = 1;

    for(auto&  child: m_children)
    {
      point  rel_pt(x,0);

      child->set_relative_point(rel_pt);

      child->reform_if_needed(get_absolute_point());


      x += child->get_width();


      w = std::max(w,rel_pt.x+child->get_width() );
      h = std::max(h,rel_pt.y+child->get_height());
    }


  m_width  = w;
  m_height = h;
}


void
table_row::
append_child(std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      container::append_child(w);
    }
}




}}




