#include"libgbscr/object.hpp"
#include<new>


namespace gbscr{
namespace objects{




property
reference::
get_property(const identifier&  id) const noexcept
{
  auto&  obj = *m_pointer;

  using sv = gbstd::string_view;

  auto  name = id->view();


  auto  fn = [](void*  ptr, const int*  v)->int{return 0;};

  return property(m_pointer,fn);
}


}}




