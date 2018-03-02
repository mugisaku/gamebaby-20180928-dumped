#include"libgbscr/streams/stream.hpp"
#include<cstring>
#include<cstdio>




namespace gbscr{


namespace{
bool
isalpha(char  c) noexcept
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')));
}


bool
isdigit(char  c) noexcept
{
  return((c >= '0') && (c <= '9'));
}


bool
isalnum(char  c) noexcept
{
  return(isalpha(c) || isdigit(c));
}


bool
isident0(char  c) noexcept
{
  return isalpha(c) || (c == '_');
}


bool
isidentn(char  c) noexcept
{
  return isalnum(c) || (c == '_');
}
}




bool
stream::
is_pointing_identifier() const noexcept
{
  return isident0(*m_pointer);
}


bool
stream::
is_pointing_number() const noexcept
{
  return isdigit(*m_pointer);
}


token
stream::
read_identifier() noexcept
{
  m_string_buffer.resize(0,0);

    while(!is_reached_end() && isidentn(*m_pointer))
    {
      m_string_buffer.push_back(*m_pointer++);
    }


  return gbstd::string_view(m_string_buffer.data(),m_string_buffer.size());
}


token
stream::
read_quoted_string(char  close_char) noexcept
{
  m_string_buffer.resize(0,0);

    while(!is_reached_end())
    {
      auto  c = *m_pointer++;

        if(c == close_char)
        {
          break;
        }

      else
        if(c == '\\')
        {
          break;
        }

      else
        {
          m_string_buffer.push_back(c);
        }
    }


  return gbstd::string_view(m_string_buffer.data(),m_string_buffer.size());
}




void
print(const char*  start, const char*  cursor) noexcept
{
  int  line_number = 1;

  auto           p = start;
  auto  line_start = start;

    while(p < cursor)
    {
      auto  c = *p++;

        if(c == '\n')
        {
          line_start = cursor;

          ++line_number;
        }
    }


  printf("[stream %4d行]\n",line_number);

  auto  p = line_start;

    while(*p && (*p != '\n'))
    {
      printf("%c",*p++);
    }


  printf("\n");


  p = line_start;

    while(p++ < (cursor-1))
    {
      printf(" ");
    }


  printf("[^]\n");
}


}




