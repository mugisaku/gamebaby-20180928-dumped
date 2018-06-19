#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
program::
clear() noexcept
{
  m_stack.clear();

  m_top = nullptr;
}


void
program::
push(context&  ctx) noexcept
{
  m_stack.emplace_back(&ctx);

  m_top = &ctx;

  ctx.reset();
}


void
program::
pop(value  v) noexcept
{
    if(m_stack.size())
    {
      m_top->remove();

      m_stack.pop_back();

        if(m_stack.size())
        {
          m_top = m_stack.back();

          m_top->set_value(std::move(v));
        }
    }
}


void
program::
step() noexcept
{
    if(m_top)
    {
      m_top->step();

        if(m_top->is_halted())
        {
          auto  v = m_top->get_value();

          pop(std::move(v));
        }
    }
}




}}




