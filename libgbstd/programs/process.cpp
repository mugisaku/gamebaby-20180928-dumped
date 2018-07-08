#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
process::
reset(programs::program&  program) noexcept
{
  m_program = &program;

  m_pc = 0;

  m_ended   = false;
  m_removed = false;
}


void
process::
call(process&  prc) noexcept
{
    if(!m_calling_count)
    {
      m_program->push(prc);

      ++m_calling_count;
    }

  else
    {
      printf("[process::call error] 多重コールはしてはならない\n");
    }
}


void
process::
end(value  v) noexcept
{
  m_end_value = std::move(v);

  m_ended = true;
}


void
process::
remove() noexcept
{
  m_removed = true;
}



}}




