#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




struct
program::
node
{
  programs::context*  context;

  void  (*onpop)(programs::context*  ctx);

  node*  previous;

};




program::node*
program::
pump_node() noexcept
{
  node*  nd;

    if(m_trash)
    {
      nd = m_trash                    ;
           m_trash = m_trash->previous;
    }

  else
    {
      nd = new node;
    }


  return nd;
}


void
program::
dump_node(node*  nd) noexcept
{
  nd->previous = m_trash     ;
                 m_trash = nd;
}




void
program::
clear() noexcept
{
    while(m_trash)
    {
      auto  prev = m_trash->previous;

      delete m_trash       ;
             m_trash = prev;
    }


    while(m_stack)
    {
      auto  prev = m_stack->previous;

        if(prev->onpop)
        {
          prev->onpop(prev->context);
        }


      delete m_stack       ;
             m_stack = prev;
    }
}


void
program::
push(context&  ctx, void  (*onpop)(context*  ctx)) noexcept
{
  auto  nd = pump_node();

  nd->previous = m_stack     ;
                 m_stack = nd;

  nd->context = &ctx;

  nd->onpop = onpop;

  m_top = &ctx;

  ctx.reset(*this);
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
            if(m_stack->onpop)
            {
              m_stack->onpop(m_top);
            }


          dump_node(m_stack)                   ;
                    m_stack = m_stack->previous;

            if(m_stack)
            {
              m_top = m_stack->context;
            }
        }
    }
}




}}




