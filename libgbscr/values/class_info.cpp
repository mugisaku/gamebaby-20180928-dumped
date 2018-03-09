#include"libgbscr/value.hpp"
#include<new>


namespace gbscr{
namespace values{




const method*
class_info::
find_method(gbstd::string_view  name) const noexcept
{
    for(auto&  m: m_method_list)
    {
        if(m.get_name() == name)
        {
          return &m;
        }
    }


  return nullptr;
}




}}




