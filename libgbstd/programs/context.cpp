#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
context::
reset(program&  prog) noexcept
{
  m_program = &prog;

  m_pc = 0;

  m_state = state::stepping;
}


void
context::
sleep(uint32_t  ms) const noexcept
{
  m_program->set_sleep_timer(ms);
}


void
context::
leave(value  v) noexcept
{
  m_state = state::left;

  m_program->store_value(std::move(v));
}


value&
context::
get_stored_value() const noexcept
{
  return m_program->get_value();
}




}}




