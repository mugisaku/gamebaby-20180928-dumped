#include"libgbscr/stream.hpp"




namespace gbscr{


void
stream::
read_binary_number() noexcept
{
  gbstd::string  s;

  ++m_pointer;

    for(;;)
    {
      auto  c = *m_pointer;

        if((c == '0') ||
           (c == '1'))
        {
          s.append(c);

          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  m_token.set_data(token_kind::binary_integer,std::move(s));
}



void
stream::
read_octal_number() noexcept
{
  gbstd::string  s;

  ++m_pointer;

    for(;;)
    {
      auto  c = *m_pointer;

        if((c >= '0') &&
           (c <= '7'))
        {
          s.append(c);

          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  m_token.set_data(token_kind::octal_integer,std::move(s));
}


void
stream::
read_decimal_number() noexcept
{
  gbstd::string  s;

    for(;;)
    {
      auto  c = *m_pointer;

        if((*m_pointer >= '0') &&
           (*m_pointer <= '9'))
        {
          s.append(c);

          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  m_token.set_data(token_kind::decimal_integer,std::move(s));
}


void
stream::
read_hexadecimal_number() noexcept
{
  gbstd::string  s;

  ++m_pointer;

    for(;;)
    {
      auto  c = *m_pointer;

        if(((c >= '0') && (c <= '9')) ||
           ((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F')))
        {
          s.append(c);

          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  m_token.set_data(token_kind::hexadecimal_integer,std::move(s));
}


void
stream::
read_number_that_begins_by_zero() noexcept
{
  auto  c = *++m_pointer;

       if((c == 'b') || (c == 'B')){    read_binary_number();}
  else if((c == 'o') || (c == 'O')){      read_octal_number();}
  else if((c == 'x') || (c == 'X')){read_hexadecimal_number();}
  else
    {
      m_token.set_data(token_kind::decimal_integer,gbstd::string("0"));
    }
}


void
stream::
read_number() noexcept
{
    if(*m_pointer == '0'){read_number_that_begins_by_zero();}
  else                   {            read_decimal_number();}
}


}




