#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
context::
reset(programs::program&  program) noexcept
{
  m_program = &program;

  m_pc = 0;

  m_ended   = false;
  m_removed = false;
}


void
context::
call(context&  ctx) noexcept
{
    if(!m_calling_count)
    {
      m_program->push(ctx);

      ++m_calling_count;
    }

  else
    {
      printf("[context::call error] 多重コールはしてはならない\n");
    }
}


void
context::
end(value  v) noexcept
{
  m_end_value = std::move(v);

  m_ended = true;
}


void
context::
remove() noexcept
{
  m_removed = true;
}



}}




