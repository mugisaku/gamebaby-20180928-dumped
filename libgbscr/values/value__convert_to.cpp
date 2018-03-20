#include"libgbscr/value.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbstd/utility.hpp"
#include<cstdlib>


namespace gbscr{
namespace values{




int
value::
convert_to_integer() const
{
    switch(m_kind)
    {
  case(kind::null):
      break;
  case(kind::integer):
      return get_integer();
      break;
  case(kind::constant_string):
      return std::strtol(m_data.cs->data(),nullptr,0);
      break;
  case(kind::string):
      return std::strtol(m_data.s.data(),nullptr,0);
      break;
  case(kind::reference):
      return m_data.r->get_value().convert_to_integer();
      break;
  case(kind::routine):
      return 1;
      break;
  case(kind::table):
      return 1;
      break;
  default:;
    }


  throw value_conversion_error{};
}


shared_string
value::
convert_to_string() const
{
  char  buf[256];

  int  n;

    switch(m_kind)
    {
  case(kind::null):
      break;
  case(kind::integer):
      n = snprintf(buf,sizeof(buf),"%d",m_data.i);

      return shared_string(buf,n);
      break;
  case(kind::constant_string):
      return shared_string(m_data.cs->data(),m_data.cs->size());
      break;
  case(kind::string):
      return m_data.s;
      break;
  case(kind::reference):
      return m_data.r->get_value().convert_to_string();
      break;
  case(kind::routine):
      break;
  case(kind::table):
      break;
  default:;
    }


  throw value_conversion_error{};
}


const stmts::routine&
value::
convert_to_routine() const
{
    switch(m_kind)
    {
  case(kind::null):
      break;
  case(kind::integer):
      break;
  case(kind::string):
      break;
  case(kind::reference):
      return m_data.r->get_value().convert_to_routine();
      break;
  case(kind::routine):
      return *m_data.rt;
      break;
  case(kind::table):
      break;
  default:;
    }


  throw value_conversion_error{};
}




}}




