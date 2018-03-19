#include"libgbscr/stream.hpp"
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




void
stream::
read_quoted_string(char  close_char) noexcept
{
  gbstd::string  s;

  ++m_pointer;

    for(;;)
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
          s.append(c);
        }
    }


  m_token.set_data(token_kind::string,std::move(s));
}


bool
stream::
read_punct(gbstd::string_view  sv) noexcept
{
    if(std::memcmp(m_pointer,sv.data(),sv.size()) == 0)
    {
      m_pointer += sv.size();

      m_token.set_data(token_kind::punctuations,gbstd::string(sv));

      return true;
    }


  return false;
}


token
stream::
read_token()
{
  m_token = token();

START:
  m_token.set_pointer(m_pointer);

    if(!*this)
    {
      return token();
    }


  auto  c = get_char();

    if(isident0(c))
    {
      gbstd::string  s;

        while(isidentn(*m_pointer))
        {
          s.append(*m_pointer++);
        }


      m_token.set_data(token_kind::identifier,std::move(s));
    }

  else
    if(isdigit(c))
    {
      read_number();
    }

  else
    if((c == '\'') ||
       (c == '\"'))
    {
      read_quoted_string(c);
    }

  else
    if(c == '(')
    {
      ++m_pointer;

      m_token.set_data(*new block(*this,'(',')'));
    }

  else
    if(c == '{')
    {
      ++m_pointer;

      m_token.set_data(*new block(*this,'{','}'));
    }

  else
    if(c == '[')
    {
      ++m_pointer;

      m_token.set_data(*new block(*this,'[',']'));
    }

  else if(read_punct("...")){}
  else if(read_punct("<<=")){}
  else if(read_punct("<<")){}
  else if(read_punct("<=")){}
  else if(read_punct("<")){}
  else if(read_punct(">>=")){}
  else if(read_punct(">>")){}
  else if(read_punct(">=")){}
  else if(read_punct(">")){}
  else if(read_punct("++")){}
  else if(read_punct("+=")){}
  else if(read_punct("+")){}
  else if(read_punct("--")){}
  else if(read_punct("-=")){}
  else if(read_punct("->")){}
  else if(read_punct("-")){}
  else if(read_punct("*=")){}
  else if(read_punct("*")){}
  else if(read_punct("/=")){}
  else if(read_punct("/")){}
  else if(read_punct("%=")){}
  else if(read_punct("%")){}
  else if(read_punct("&&")){}
  else if(read_punct("&=")){}
  else if(read_punct("&")){}
  else if(read_punct("||")){}
  else if(read_punct("|=")){}
  else if(read_punct("|")){}
  else if(read_punct("^=")){}
  else if(read_punct("^")){}
  else if(read_punct("::")){}
  else if(read_punct(":")){}
  else if(read_punct("==")){}
  else if(read_punct("=")){}
  else if(read_punct("!=")){}
  else if(read_punct("!")){}
  else if(read_punct("?")){}
  else if(read_punct(",")){}
  else if(read_punct(".")){}
  else if(read_punct(";")){}
  else if(read_punct("")){}
  else if(read_punct("~")){}
  else
    if((c ==  ' ') ||
       (c == '\t') ||
       (c == '\r') ||
       (c == '\n'))
    {
      skip_spaces();

      goto START;
    }

  else
    {
      printf("処理できない文字です %d\n",c);
    }


  return std::move(m_token);
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

  p = line_start;

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




