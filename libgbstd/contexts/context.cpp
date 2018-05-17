#include"libgbstd/context.hpp"


namespace gbstd{
namespace contexts{




void
context::
reset(context_stack&  stack) noexcept
{
  m_stack = &stack;

  m_pc = 0;

  m_state = state::stepping;

  m_stored_value = 0;
}


void
context::
sleep(uint32_t  ms) const noexcept
{
  m_stack->set_sleep_timer(ms);
}


void
context::
finish(int  v) noexcept
{
  m_state = state::finished;

  m_stored_value = v;
}




}}




