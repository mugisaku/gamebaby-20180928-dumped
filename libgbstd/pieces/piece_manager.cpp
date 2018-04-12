#include"libgbstd/piece.hpp"




namespace gbstd{
namespace pieces{




void
piece_manager::
clear()  noexcept
{
  auto  ptr = m_first;

    while(ptr)
    {
      auto  next = ptr->get_next();

      delete ptr       ;
             ptr = next;
    }


  m_first = nullptr;
  m_last  = nullptr;
}


void
piece_manager::
append(piece*  p, point  pt) noexcept
{
    if(p)
    {
        if(p->get_manager())
        {
          printf("piece manager append error\n");

          return;
        }


      p->set_manager(this);

        if(m_last)
        {
          m_last->set_next(p);

          p->set_previous(m_last);
        }

      else
        {
          m_first = p;
        }


      m_last = p;
    }
}


void
piece_manager::
remove(piece*  p) noexcept
{
    if(p)
    {
        if(p == m_first)
        {
          m_first = p->get_next();

            if(m_first)
            {
              m_first->set_previous(nullptr);
            }

          else
            {
              m_last = nullptr;
            }
        }

      else
        if(p == m_last)
        {
          m_last = p->get_previous();

          m_last->set_next(nullptr);
        }

      else
        {
          auto  prev = p->get_previous();
          auto  next = p->get_next();

            if(prev)
            {
              prev->set_next(next);
            }


            if(next)
            {
              next->set_previous(prev);
            }
        }
    }
}


void
piece_manager::
composite(image&  dst) noexcept
{
  auto  ptr = m_first;

    while(ptr)
    {
      ptr->render(dst);

      ptr = ptr->get_next();
    }
}




}}




