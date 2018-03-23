#include"libgbscr/value.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbstd/utility.hpp"
#include<cstdlib>


namespace gbscr{
namespace values{




bool
value::
convert_to_boolean() const
{
    if(is_boolean())
    {
      return get_boolean();
    }

  else
    if(is_integer())
    {
      return get_integer();
    }

  else
    if(is_string())
    {
      return std::strtol(get_string().data(),nullptr,0);
    }

  else
    if(is_reference())
    {
      return get_reference()->get_operand().get_value().convert_to_boolean();
    }

  else
    if(is_routine())
    {
      return 1;
    }

  else
    if(is_table())
    {
      return 1;
    }


  throw value_conversion_error{};
}


int
value::
convert_to_integer() const
{
    if(is_boolean())
    {
      return get_boolean()? 1:0;
    }

  else
    if(is_integer())
    {
      return get_integer();
    }

  else
    if(is_string())
    {
      return std::strtol(get_string().data(),nullptr,0);
    }

  else
    if(is_reference())
    {
      return get_reference()->get_operand().get_value().convert_to_integer();
    }

  else
    if(is_routine())
    {
      return 1;
    }

  else
    if(is_table())
    {
      return 1;
    }


  throw value_conversion_error{};
}


shared_string
value::
convert_to_string() const
{
  char  buf[256];

  int  n;

    if(is_boolean())
    {
    }

  else
    if(is_integer())
    {
      n = snprintf(buf,sizeof(buf),"%d",get_integer());

      return shared_string(buf,n);
    }

  else
    if(is_string())
    {
      return shared_string(get_string());
    }

  else
    if(is_reference())
    {
      return get_reference()->get_operand().get_value().convert_to_string();
    }

  else
    if(is_routine())
    {
    }

  else
    if(is_table())
    {
    }


  throw value_conversion_error{};
}


const stmts::routine&
value::
convert_to_routine() const
{
    if(is_boolean())
    {
    }

  else
    if(is_integer())
    {
    }

  else
    if(is_string())
    {
    }

  else
    if(is_reference())
    {
      return get_reference()->get_operand().get_value().convert_to_routine();
    }

  else
    if(is_routine())
    {
      return get_routine();
    }

  else
    if(is_table())
    {
    }


  throw value_conversion_error{};
}




}}




