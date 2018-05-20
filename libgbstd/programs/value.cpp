#include"libgbstd/program.hpp"


namespace gbstd{
namespace programs{




value&
value::
operator=(int  i) noexcept
{
  clear();

  new(&m_data) int(i);

  m_kind = kind::integer;

  return *this;
}


value&
value::
operator=(gbstd::string_view  sv) noexcept
{
  clear();

  new(&m_data) gbstd::string(sv);

  m_kind = kind::string;

  return *this;
}


value&
value::
operator=(const value&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::integer): m_data.i = rhs.m_data.i;break;
      case(kind::string ): new(&m_data) gbstd::string(rhs.m_data.s);break;
        }
    }


  return *this;
}


value&
value::
operator=(value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::integer): m_data.i = rhs.m_data.i;break;
      case(kind::string ): new(&m_data) gbstd::string(std::move(rhs.m_data.s));break;
        }
    }


  return *this;
}




void
value::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::integer): break;
  case(kind::string ): gbstd::destruct(m_data.s);break;
    }


  m_kind = kind::null;
}




}}




