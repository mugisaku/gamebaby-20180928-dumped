#include"libgbscr/object.hpp"
#include"libgbscr/execution.hpp"
#include"libgbscr/stmt.hpp"
#include<new>


namespace gbscr{
namespace objects{




value&
value::
operator=(bool  b) noexcept
{
  clear();

  m_kind = kind::integer;

  m_data.i = b? 1:0;

  return *this;
}


value&
value::
operator=(int  i) noexcept
{
  clear();

  m_kind = kind::integer;

  m_data.i = i;

  return *this;
}


value&
value::
operator=(reference  r) noexcept
{
  clear();

  m_kind = kind::reference;

  m_data.r = r;

  return *this;
}


value&
value::
operator=(const stmts::routine&  rt) noexcept
{
  clear();

  m_kind = kind::routine;

  m_data.rt = &rt;

  return *this;
}


value&
value::
operator=(const property&  pr) noexcept
{
  clear();

  m_kind = kind::property;

  m_data.pr = pr;

  return *this;
}


value&
value::
operator=(system  sys) noexcept
{
  clear();

  m_kind = kind::system;

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
      case(kind::integer):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::reference):
          m_data.r = rhs.m_data.r;
          break;
      case(kind::routine):
          m_data.rt = rhs.m_data.rt;
          break;
      case(kind::property):
          m_data.pr = rhs.m_data.pr;
          break;
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
      case(kind::integer):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::reference):
          m_data.r = rhs.m_data.r;
          break;
      case(kind::routine):
          m_data.rt = rhs.m_data.rt;
          break;
      case(kind::property):
          m_data.pr = rhs.m_data.pr;
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
  case(kind::integer):
      break;
  case(kind::reference):
      break;
  case(kind::routine):
      break;
  case(kind::property):
      break;
    }


  m_kind = kind::null;
}


const stmts::routine&
value::
get_routine() const noexcept
{
  return *m_data.rt;
}


int
value::
get_integer_safely() const noexcept
{
    switch(m_kind)
    {
  case(kind::integer):
      return m_data.i;
      break;
  case(kind::reference):
      return m_data.r().get_integer_safely();
      break;
  case(kind::routine):
      printf("ルーチンは、整数になれない\n");
      break;
  case(kind::property):
      return m_data.pr.get();
      break;
    }


  return 0;
}




void
value::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null):
      printf("null");
      break;
  case(kind::integer):
      printf("%d",m_data.i);
      break;
  case(kind::reference):
      printf("reference ");

      m_data.r().print();
      break;
  case(kind::routine):
      printf("routine");
      m_data.rt->print();
      break;
  case(kind::property):
      printf("property{ %d }",m_data.pr.get());
      break;
  case(kind::system):
      printf("system");
      break;
  default:
      printf("unknown value kind\n");
      break;
    }
}


}}




