#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
table_column::
append(std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      container::append_child(w,0,m_tail);

      m_tail += m_interval;
    }
}


void
table_row::
append(std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      container::append_child(w,m_tail,0);

      m_tail += m_interval;
    }
}




}}




