#include"libgbstd/context.hpp"


namespace gbstd{
namespace contexts{




void
context_stack::
push(context&  ctx) noexcept
{
  m_data.emplace_back(&ctx);

  ctx.reset(*this);
}


void
context_stack::
set_sleep_timer(uint32_t  ms) noexcept
{
  m_sleeping = true;

  m_start_time = (m_current_time+ms);
}


void
context_stack::
touch(control_device&  ctrl) noexcept
{
  m_current_time = ctrl.time;

    if(m_sleeping)
    {
        if(m_current_time < m_start_time)
        {
          return;
        }


      m_sleeping = false;
    }


START:
    if(m_top)
    {
        if(m_top->is_stepping())
        {
          m_top->step(ctrl);
        }

      else
        if(m_top->is_finished())
        {
          auto  v = m_top->load();

          m_data.pop_back();

          m_top->do_when_be_popped();

            if(m_data.size())
            {
              m_top = m_data.back();

              m_top->store(v);
            }

          else
            {
              m_top = nullptr;
            }
        }
    }

  else
    if(m_data.size())
    {
      m_top = m_data.back();

      goto START;
    }
}




}}




