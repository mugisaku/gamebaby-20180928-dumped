#include"libgbscr/dtree.hpp"
#include<cstdio>




namespace  gbscr{
namespace dtrees{




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
      case(kind_type::null):
          break;
      case(kind_type::string):
          new(&m_data) shared_string(std::move(rhs.m_data.s));
          break;
      case(kind_type::integer):
          m_data.i = rhs.m_data.i;
          break;
      case(kind_type::real):
          m_data.r = rhs.m_data.r;
          break;
      case(kind_type::list):
          new(&m_data) list(std::move(rhs.m_data.ls));
          break;
        }
    }


  return *this;
}


value&
value::
operator=(const value&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind_type::null):
          break;
      case(kind_type::string):
          new(&m_data) shared_string(rhs.m_data.s);
          break;
      case(kind_type::integer):
          m_data.i = rhs.m_data.i;
          break;
      case(kind_type::real):
          m_data.r = rhs.m_data.r;
          break;
      case(kind_type::list):
          new(&m_data) list(rhs.m_data.ls);
          break;
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
  case(kind_type::null):
      break;
  case(kind_type::string):
      m_data.s.~shared_string();
      break;
  case(kind_type::integer):
  case(kind_type::real):
      break;
  case(kind_type::list):
      m_data.ls.~list();
      break;
    }


  m_kind = kind_type::null;
}




void
value::
print(int  indent) const noexcept
{
    switch(m_kind)
    {
  case(kind_type::null):
      printf("(null)");
      break;
  case(kind_type::string):
//      m_data.s.print(indent);
      break;
  case(kind_type::integer):
      printf("%d",m_data.i);
      break;
  case(kind_type::real):
      printf("%f",m_data.r);
      break;
  case(kind_type::list):
      m_data.ls.print(indent);
      break;
    }
}




}}




