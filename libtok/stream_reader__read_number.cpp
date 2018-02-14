#include"stream_reader.hpp"




namespace tok{
namespace tok_types{


uint64_t
stream_reader::
read_binary_number() noexcept
{
  uint64_t  i = 0;

    while(!is_reached_end())
    {
      auto  c = *m_pointer;

        if((c == '0') ||
           (c == '1'))
        {
          i <<= 1;

            if(*m_pointer == '1')
            {
              i |= 1;
            }


          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  return i;
}



uint64_t
stream_reader::
read_octal_number() noexcept
{
  uint64_t  i = 0;

    while(!is_reached_end()   &&
          (*m_pointer >= '0') &&
          (*m_pointer <= '7'))
    {
      i <<= 3;

      i |= (*m_pointer++)-'0';
    }


  return i;
}


uint64_t
stream_reader::
read_decimal_number() noexcept
{
  uint64_t  i = 0;

    while(!is_reached_end() &&
          (*m_pointer >= '0') &&
          (*m_pointer <= '9'))
    {
      i *= 10;

      i += (*m_pointer++)-'0';
    }


  return i;
}


uint64_t
stream_reader::
read_hexadecimal_number() noexcept
{
  uint64_t  i = 0;

    while(!is_reached_end())
    {
      auto  c = *m_pointer;

        if((c >= '0') &&
           (c <= '9'))
        {
          i <<= 4;

          i += c-'0';

          ++m_pointer;
        }

      else
        if(((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F')))
        {
          i <<= 4;

            switch(c)
            {
          case('a'):
          case('A'): i |= 10;break;
          case('b'):
          case('B'): i |= 11;break;
          case('c'):
          case('C'): i |= 12;break;
          case('d'):
          case('D'): i |= 13;break;
          case('e'):
          case('E'): i |= 14;break;
          case('f'):
          case('F'): i |= 15;break;
            }


          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  return i;
}


uint64_t
stream_reader::
read_number_that_begins_by_zero() noexcept
{
  auto  c = *m_pointer++;

  return ((c == 'b') || (c == 'B'))? read_binary_number()
        :((c == 'o') || (c == 'O'))? read_octal_number()
        :((c == 'x') || (c == 'X'))? read_hexadecimal_number()
        : 0;
}


uint64_t
stream_reader::
read_number() noexcept
{
  return (*m_pointer == '0')? read_number_that_begins_by_zero(): read_decimal_number();
}


}}




