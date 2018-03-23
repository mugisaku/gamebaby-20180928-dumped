#include"libgbscr/expr.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbscr/value.hpp"
#include"libgbscr/process.hpp"


namespace gbscr{
namespace exprs{




bool
read_operand(cursor&  cur, operand&  o, process&  proc)
{
    if(cur->is_binary_integer())
    {
      auto&  s = cur++->get_string();

      uint64_t  i = 0;

        for(auto  c: s)
        {
          i <<=     1;
          i  |= c-'0';
        }


      o = value(static_cast<int>(i));
    }

  else
    if(cur->is_octal_integer())
    {
      auto&  s = cur++->get_string();

      uint64_t  i = 0;

        for(auto  c: s)
        {
          i <<=     3;
          i  |= c-'0';
        }


      o = value(static_cast<int>(i));
    }

  else
    if(cur->is_decimal_integer())
    {
      auto&  s = cur++->get_string();

      uint64_t  i = 0;

        for(auto  c: s)
        {
          i *=    10;
          i |= c-'0';
        }


      o = value(static_cast<int>(i));
    }

  else
    if(cur->is_hexadecimal_integer())
    {
      auto&  s = cur++->get_string();

      uint64_t  i = 0;

        for(auto  c: s)
        {
          i <<= 4;

            if((c >= '0') &&
               (c <= '9'))
            {
              i  |= c-'0';
            }

          else
            {
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
            }
        }


      o = value(static_cast<int>(i));
    }

  else
    if(cur->is_string())
    {
      o = value(gbstd::string(cur++->get_string()));
    }

  else
    if(cur->is_identifier())
    {
      auto&  s = cur->get_string();

           if(s == gbstd::string_view( "null")){  ++cur;  o = value();}
      else if(s == gbstd::string_view( "true")){  ++cur;  o = value(true);}
      else if(s == gbstd::string_view("false")){  ++cur;  o = value(false);}
      else
        if(s == gbstd::string_view("routine"))
        {
            if(cur[1].is_block('(',')') &&
               cur[2].is_block('{','}'))
            {
              auto&  parals_blk = cur[1].get_block();
              auto&    impl_blk = cur[2].get_block();

              o = value(routine(parals_blk,impl_blk,proc));

              cur += 3;
            }

          else
            {
              return false;
            }
        }

      else
        if(s == gbstd::string_view("table"))
        {
            if(cur[1].is_block('{','}'))
            {
              o = value(table(cur[1].get_block(),proc));

              cur += 2;
            }

          else
            {
              return false;
            }
        }

      else
        {
          o = identifier(s.view());

          cur += 1;
        }
    }

  else
    {
      return false;
    }


  return true;
	}




}}




