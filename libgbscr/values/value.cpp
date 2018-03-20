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
operator=(const gbstd::string&  s) noexcept
{
  clear();

  m_kind = kind::constant_string;

  m_data.cs = &s;

  return *this;
}


value&
value::
operator=(const shared_string&  s) noexcept
{
  clear();

  m_kind = kind::string;

  new(&m_data) shared_string(s);

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
operator=(const table&  tbl) noexcept
{
  clear();

  m_kind = kind::table;

  new(&m_data) table_observer(tbl.get_observer());

  return *this;
}


value&
value::
operator=(const table_observer&  obs) noexcept
{
  clear();

  m_kind = kind::table;

  new(&m_data) table_observer(obs);

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
      case(kind::constant_string):
          m_data.cs = rhs.m_data.cs;
          break;
      case(kind::string):
          new(&m_data) shared_string(rhs.m_data.s);
          break;
      case(kind::reference):
          new(&m_data) reference(rhs.m_data.r);
          break;
      case(kind::routine):
          m_data.rt = rhs.m_data.rt;
          break;
      case(kind::table):
          new(&m_data) table_observer(rhs.m_data.obs);
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
      case(kind::constant_string):
          m_data.cs = rhs.m_data.cs;
          break;
      case(kind::string):
          new(&m_data) shared_string(std::move(rhs.m_data.s));
          break;
      case(kind::reference):
          new(&m_data) reference(std::move(rhs.m_data.r));
          break;
      case(kind::routine):
          m_data.rt = rhs.m_data.rt;
          break;
      case(kind::table):
          new(&m_data) table_observer(std::move(rhs.m_data.obs));
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
  case(kind::string):
      gbstd::destruct(m_data.s);
      break;
  case(kind::reference):
      gbstd::destruct(m_data.r);
      break;
  case(kind::routine):
      break;
  case(kind::table):
      gbstd::destruct(m_data.obs);
      break;
    }


  m_kind = kind::null;
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
  case(kind::constant_string):
      printf("constant string\"%s\"",m_data.cs->data());
      break;
  case(kind::string):
      printf("shared string\"");
      m_data.s.print();
      printf("\"");
      break;
  case(kind::reference):
      printf("reference ");

      m_data.r->print();
      break;
  case(kind::routine):
      printf("routine");
      m_data.rt->print();
      break;
  case(kind::table):
      printf("table");
      break;
  default:
      printf("unknown value kind\n");
      break;
    }
}


}}




