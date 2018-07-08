#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
program::
clear() noexcept
{
    for(auto  prc: m_stack)
    {
      prc->remove();
    }


  m_stack.clear();

  m_top = nullptr;
}


void
program::
push(process&  prc) noexcept
{
  m_stack.emplace_back(&prc);

  m_top = &prc;

  prc.reset(*this);
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

          m_top->set_end_value(std::move(v));
        }

      else
        {
          m_top = nullptr;
        }
    }
}


void
program::
step() noexcept
{
  m_top->ready_to_step();

  m_top->step();

    if(m_top->is_ended())
    {
      auto&  v = m_top->get_end_value();

      pop(value(v));
    }
}




}}




