#include"libgbscr/value.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbstd/utility.hpp"
#include<new>


namespace gbscr{
namespace values{




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
operator=(const reference&  r) noexcept
{
  clear();

  m_kind = kind::reference;

  new(&m_data) reference(r);

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
operator=(const object&  obj) noexcept
{
  clear();

  m_kind = kind::object;

  new(&m_data) object(obj);

  return *this;
}


value&
value::
operator=(const method_calling&  mc) noexcept
{
  clear();

  m_kind = kind::method_calling;

  m_data.mc = mc;

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
          new(&m_data) reference(rhs.m_data.r);
          break;
      case(kind::routine):
          m_data.rt = rhs.m_data.rt;
          break;
      case(kind::object):
          new(&m_data) object(rhs.m_data.obj);
          break;
      case(kind::method_calling):
          m_data.mc = rhs.m_data.mc;
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
          new(&m_data) reference(std::move(rhs.m_data.r));
          break;
      case(kind::routine):
          m_data.rt = rhs.m_data.rt;
          break;
      case(kind::object):
          new(&m_data) object(std::move(rhs.m_data.obj));
          break;
      case(kind::method_calling):
          m_data.mc = rhs.m_data.mc;
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
      gbstd::destruct(m_data.r);
      break;
  case(kind::routine):
      break;
  case(kind::object):
      gbstd::destruct(m_data.obj);
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
  case(kind::method_calling):
      printf("メソッド呼び出しは、整数になれない\n");
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
  case(kind::method_calling):
      printf("method calling");
      break;
  case(kind::object):
      printf("object");
      break;
  default:
      printf("unknown value kind\n");
      break;
    }
}


}}




