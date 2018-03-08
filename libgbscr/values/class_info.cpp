#include"libgbscr/value.hpp"
#include<new>


namespace gbscr{
namespace values{




const method*
class_info::
find_method(gbstd::string_view  name) const noexcept
{
    for(auto  it = m_begin;  it != m_end;  ++it)
    {
        if(it->get_name() == name)
        {
          return it;
        }
    }


  return nullptr;
}




}}




