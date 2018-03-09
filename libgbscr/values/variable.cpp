#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{




variable::
variable(const value&  v, gbstd::string_view  name) noexcept:
m_value(v),
m_name(name)
{
}




void
variable::
set_carry_flag() noexcept
{
    if(!m_carry_flag)
    {
      m_carry_flag = true;
    }
}


void
variable::
print() const noexcept
{
  printf("%s ",m_name.data());

  m_value.print();
}




}}




