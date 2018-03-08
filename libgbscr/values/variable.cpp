#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{




variable::
variable(const value&  v, gbstd::string_view  name) noexcept:
reference(m_value),
m_value(v),
m_name(name)
{
}


variable::
~variable()
{
    if(reference::get_count() > 1)
    {
      reference::unset_pointer();
    }
}


void
variable::
print() const noexcept
{
  printf("%d \n",get_count());
}




}}




