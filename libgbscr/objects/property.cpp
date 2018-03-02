#include"libgbsnd/object.hpp"
#include<new>


namespace gbsnd{
namespace objects{




int
property::
get() const noexcept
{
  return m_callback(m_pointer,nullptr);
}


void
property::
set(int  v) const noexcept
{
  m_callback(m_pointer,&v);
}




}}




