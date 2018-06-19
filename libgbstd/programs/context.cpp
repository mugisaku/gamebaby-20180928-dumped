#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
context::
reset() noexcept
{
  m_pc = 0;

  m_halted  = false;
  m_removed = false;
}


void
context::
halt(value  v) noexcept
{
  m_value = std::move(v);

  m_halted = true;
}


void
context::
remove() noexcept
{
  m_removed = true;

  do_when_removed();
}



}}




