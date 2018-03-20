#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{




void
table_observer::
unrefer() noexcept
{
    if(m_link)
    {
        if(!--m_link->count)
        {
          delete m_link;
        }


      m_link = nullptr;
    }
}




table_observer&
table_observer::
operator=(const table_observer&   rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_link = rhs.m_link;

        if(m_link)
        {
          ++m_link->count;
        }
    }


  return *this;
}


table_observer&
table_observer::
operator=(table_observer&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_link,rhs.m_link);
    }


  return *this;
}




}}




