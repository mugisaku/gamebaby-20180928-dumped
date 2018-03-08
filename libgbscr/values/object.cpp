#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{


method_calling
object::
operator[](gbstd::string_view  name) const noexcept
{
  auto  m = m_info.find_method(name);

  return method_calling{m_data,m};
}


}}




