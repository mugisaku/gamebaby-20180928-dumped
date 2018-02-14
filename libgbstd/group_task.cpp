#include"libgbstd/task.hpp"


namespace gbstd{
namespace tasks{




void
group_task::
push_to_first(task&  t) noexcept
{
    if(!t.get_group())
    {
        if(m_first)
        {
          t.connect(*m_first);
          m_first = &t;
        }

      else
        {
          m_first = &t;
           m_last = &t;
        }


      t.set_group(*this);

      ++m_number_of_tasks;
    }
}


void
group_task::
push_to_last(task&  t) noexcept
{
    if(!t.get_group())
    {
        if(m_last)
        {
          m_last->connect(t);
          m_last = &t;
        }

      else
        {
          m_first = &t;
           m_last = &t;
        }


      t.set_group(*this);

      ++m_number_of_tasks;
    }
}


void
group_task::
erase(task&  t) noexcept
{
    if(t.get_group())
    {
        if(m_first == &t)
        {
          m_first = m_first->get_next();
        }


        if(m_last == &t)
        {
          m_last = m_last->get_previous();
        }


      t.disconnect();

      --m_number_of_tasks;
    }
}


rw_ptr<task>
group_task::
find_by_name(gbstd::string_view  name) const noexcept
{
  auto  next = m_first;

    while(next)
    {
        if(next->get_name() == name)
        {
          return next;
        }

      else
        if(next->is_group())
        {
          rw_ptr<group_task>  cogrp(next);

          auto  res = cogrp->find_by_name(name);

            if(res)
            {
              return res;
            }
        }


      next = next->get_next();
    }


  return nullptr;
}


void
group_task::
update() noexcept
{
  auto  current = m_first;

    while(current)
    {
      current->update();

      current = current->get_next();
    }
}


void
group_task::
render(image&  dst, point  offset) const noexcept
{
  auto  current = m_first.get_ro();

  offset += get_base_point();

    while(current)
    {
      current->render(dst,offset);

      current = current->get_ro_next();
    }
}


void
group_task::
print() noexcept
{
  auto  current = m_first.get_ro();

  printf("[print task]\n");

    while(current)
    {
      printf("%s\n",current->get_name().data());

      current = current->get_ro_next();
    }


  printf("end\n\n");
}




}}




