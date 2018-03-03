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




identifier
stream::
read_identifier() noexcept
{
  m_string_buffer.resize(0,0);

    while(isidentn(*m_pointer))
    {
      m_string_buffer.push_back(*m_pointer++);
    }


  return identifier(shared_string(m_string_buffer.data(),m_string_buffer.size()));
}


shared_string
stream::
read_quoted_string(char  close_char) noexcept
{
  m_string_buffer.resize(0,0);

    while(*this)
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


  return shared_string(m_string_buffer.data(),m_string_buffer.size());
}


bool
stream::
read_operator(gbstd::string_view  sv) noexcept
{
    if(std::memcmp(m_pointer,sv.data(),sv.size()) == 0)
    {
      m_token.set_data(operator_word(sv));

      m_pointer += sv.size();

      return true;
    }


  return false;
}


const token&
stream::
read_token()
{
  m_token.unset_data();

START:
  m_token.set_pointer(m_pointer);

    if(!*this)
    {
      return m_token;
    }


  auto  c = get_char();

    if(c == ';')
    {
      advance();

      m_token.set_data(semicolon{});
    }

  else
    if(c == '(')
    {
      advance();

      m_token.set_data(token_string(*this,'(',')'));
    }

  else
    if(c == '{')
    {
      advance();

      m_token.set_data(token_string(*this,'{','}'));
    }

  else
    if(c == '[')
    {
      advance();

      m_token.set_data(token_string(*this,'[',']'));
    }

  else
    if((c == ')') ||
       (c == '}') ||
       (c == ']'))
    {
      printf("token_string error\n");

      throw m_token.get_pointer();
    }

  else if(read_operator(gbstd::string_view("..."))){}
  else if(read_operator(gbstd::string_view("."))){}
  else if(read_operator(gbstd::string_view("++"))){}
  else if(read_operator(gbstd::string_view("+="))){}
  else if(read_operator(gbstd::string_view("+"))){}
  else if(read_operator(gbstd::string_view("--"))){}
  else if(read_operator(gbstd::string_view("->"))){}
  else if(read_operator(gbstd::string_view("-="))){}
  else if(read_operator(gbstd::string_view("-"))){}
  else if(read_operator(gbstd::string_view("*="))){}
  else if(read_operator(gbstd::string_view("*"))){}
  else if(read_operator(gbstd::string_view("/="))){}
  else if(read_operator(gbstd::string_view("/"))){}
  else if(read_operator(gbstd::string_view("%="))){}
  else if(read_operator(gbstd::string_view("%"))){}
  else if(read_operator(gbstd::string_view("<<="))){}
  else if(read_operator(gbstd::string_view("<<"))){}
  else if(read_operator(gbstd::string_view("<="))){}
  else if(read_operator(gbstd::string_view("<"))){}
  else if(read_operator(gbstd::string_view(">>="))){}
  else if(read_operator(gbstd::string_view(">>"))){}
  else if(read_operator(gbstd::string_view(">="))){}
  else if(read_operator(gbstd::string_view(">"))){}
  else if(read_operator(gbstd::string_view("||"))){}
  else if(read_operator(gbstd::string_view("|="))){}
  else if(read_operator(gbstd::string_view("|"))){}
  else if(read_operator(gbstd::string_view("&&"))){}
  else if(read_operator(gbstd::string_view("&="))){}
  else if(read_operator(gbstd::string_view("&"))){}
  else if(read_operator(gbstd::string_view("^="))){}
  else if(read_operator(gbstd::string_view("^"))){}
  else if(read_operator(gbstd::string_view("=="))){}
  else if(read_operator(gbstd::string_view("="))){}
  else if(read_operator(gbstd::string_view("!="))){}
  else if(read_operator(gbstd::string_view("!"))){}
  else if(read_operator(gbstd::string_view("::"))){}
  else if(read_operator(gbstd::string_view(":"))){}
  else if(read_operator(gbstd::string_view(","))){}
  else if(read_operator(gbstd::string_view("?"))){}
  else
    if(isident0(c))
    {
      m_token.set_data(read_identifier());
    }

  else
    if(isdigit(c))
    {
      m_token.set_data(read_number());
    }

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


  return m_token;
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




