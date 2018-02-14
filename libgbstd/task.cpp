#include"task.hpp"




namespace gbstd{
namespace tasks{




void
task::
connect(task&  new_next) noexcept
{
  new_next.m_previous = this;

  m_next = &new_next;
}


void
task::
disconnect() noexcept
{
    if(m_previous)
    {
      m_previous->m_next = m_next;
    }


    if(m_next)
    {
      m_next->m_previous = m_previous;
    }


  m_previous = nullptr;
      m_next = nullptr;

  m_group = nullptr;
}




}}




