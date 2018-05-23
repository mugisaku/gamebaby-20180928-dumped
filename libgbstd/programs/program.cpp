#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




void
program::
clear() noexcept
{
    while(m_node_list.size())
    {
      pop();
    }
}


void
program::
push(context&  ctx, void  (*onpop)(context*  ctx)) noexcept
{
  m_node_list.emplace_back(node{&ctx,onpop});

  m_top = &ctx;

  ctx.reset(*this);
}


void
program::
pop() noexcept
{
  auto&  node = m_node_list.back();

    if(node.onpop)
    {
      node.onpop(node.context);
    }


  m_node_list.pop_back();
}


void
program::
set_sleep_timer(uint32_t  ms) noexcept
{
  m_sleeping = true;

  m_start_time = (m_current_time+ms);
}


void
program::
touch(uint32_t  time) noexcept
{
  m_current_time = time;

    if(m_sleeping)
    {
        if(m_current_time < m_start_time)
        {
          return;
        }


      m_sleeping = false;
    }


    if(m_top)
    {
        if(m_top->is_stepping())
        {
          m_top->step();
        }

      else
        if(m_top->is_left())
        {
          pop();

            if(m_node_list.size())
            {
              m_top = m_node_list.back().context;
            }
        }
    }
}




}}




