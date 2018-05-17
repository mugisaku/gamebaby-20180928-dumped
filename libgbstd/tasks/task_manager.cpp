#include"libgbstd/task.hpp"


namespace gbstd{
namespace tasks{




struct
task_manager::
node
{
  tasks::task*  task;
  node*         next;

};


task_manager::node*
task_manager::
get_node() noexcept
{
    if(m_trash)
    {
      auto  nd = m_trash      ;
                 m_trash->next;

      return nd;
    }

  else
    {
      return new node;
    }
}


void
task_manager::
dump_node(node*  nd) noexcept
{
  nd->next = m_trash     ;
             m_trash = nd;
}




void
task_manager::
clear() noexcept
{
  auto  next = m_first;

    while(next)
    {
      auto  node = next             ;
                   next = next->next;

      auto&  task = *node->task;

      dump_node(node);

      task.do_when_remove();
    }
}


void
task_manager::
push_front(task*  t) noexcept
{
    if(t)
    {
      auto  nd = get_node();

      nd->task = t;

      nd->next = m_first     ;
                 m_first = nd;

        if(!m_last)
        {
          m_last = nd;
        }
    }
}


void
task_manager::
push_back(task*  t) noexcept
{
    if(t)
    {
      auto  nd = get_node();

      nd->task = t;

      nd->next = nullptr;

        if(m_last)
        {
          m_last->next = nd;
        }

      else
        {
          m_first = nd;
        }


      m_last = nd;
    }
}


void
task_manager::
update() noexcept
{
  auto  next = m_first;

    while(next)
    {
      auto  node = next             ;
                   next = next->next;

      auto&  task = *node->task;

        if(task.is_needed_to_remove())
        {
          dump_node(node);

          task.do_when_remove();
        }

      else
        {
          task.update();
        }
    }
}


}}




