#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{




variable::
variable(table&  table, const value&  v, gbstd::string_view  name) noexcept:
m_table(&table),
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

      auto&  v = m_value.is_reference()? m_value.get_reference()().get_value()
                                       : m_value;

        if(v.is_table())
        {
          v.get_table().set_carry_flag();
        }
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




