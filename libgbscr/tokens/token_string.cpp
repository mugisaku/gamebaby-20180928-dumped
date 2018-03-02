#include"libgbscr/token.hpp"
#include<vector>


namespace  gbscr{
namespace tokens{




namespace{
bool
try_push(stream&  r, gbstd::string_view  sv, std::vector<token>&  buf) noexcept
{
  operator_word  opw(sv);

    if(r.advance_if_matched(sv))
    {
      buf.emplace_back(opw);

      return true;
    }


  return false;
}
}


token_string::
token_string(stream&  r, char  open, char  close):
m_open(open),
m_close(close)
{
  std::vector<token>  buffer;

    for(;;)
    {
      r.skip_spaces();

        if(r.is_reached_end())
        {
            if(open)
            {
              printf("token_string error\n");
            }


          break;
        }


      auto  c = r.get_char();

      const auto  p = r.get_pointer();

        if(c == close)
        {
          r.advance();

          break;
        }

      else
        if(c == ';')
        {
          r.advance();

          buffer.emplace_back(semicolon{});
        }

      else
        if(c == '(')
        {
          r.advance();

          buffer.emplace_back(token_string(r,'(',')'));
        }

      else
        if(c == '{')
        {
          r.advance();

          buffer.emplace_back(token_string(r,'{','}'));
        }

      else
        if(c == '[')
        {
          r.advance();

          buffer.emplace_back(token_string(r,'[',']'));
        }

      else
        if((c == ')') ||
           (c == '}') ||
           (c == ']'))
        {
          printf("token_string error\n");

          throw;
        }

      else if(try_push(r,gbstd::string_view("..."),buffer)){}
      else if(try_push(r,gbstd::string_view("."),buffer)){}
      else if(try_push(r,gbstd::string_view("++"),buffer)){}
      else if(try_push(r,gbstd::string_view("+="),buffer)){}
      else if(try_push(r,gbstd::string_view("+"),buffer)){}
      else if(try_push(r,gbstd::string_view("--"),buffer)){}
      else if(try_push(r,gbstd::string_view("->"),buffer)){}
      else if(try_push(r,gbstd::string_view("-="),buffer)){}
      else if(try_push(r,gbstd::string_view("-"),buffer)){}
      else if(try_push(r,gbstd::string_view("*="),buffer)){}
      else if(try_push(r,gbstd::string_view("*"),buffer)){}
      else if(try_push(r,gbstd::string_view("/="),buffer)){}
      else if(try_push(r,gbstd::string_view("/"),buffer)){}
      else if(try_push(r,gbstd::string_view("%="),buffer)){}
      else if(try_push(r,gbstd::string_view("%"),buffer)){}
      else if(try_push(r,gbstd::string_view("<<="),buffer)){}
      else if(try_push(r,gbstd::string_view("<<"),buffer)){}
      else if(try_push(r,gbstd::string_view("<="),buffer)){}
      else if(try_push(r,gbstd::string_view("<"),buffer)){}
      else if(try_push(r,gbstd::string_view(">>="),buffer)){}
      else if(try_push(r,gbstd::string_view(">>"),buffer)){}
      else if(try_push(r,gbstd::string_view(">="),buffer)){}
      else if(try_push(r,gbstd::string_view(">"),buffer)){}
      else if(try_push(r,gbstd::string_view("||"),buffer)){}
      else if(try_push(r,gbstd::string_view("|="),buffer)){}
      else if(try_push(r,gbstd::string_view("|"),buffer)){}
      else if(try_push(r,gbstd::string_view("&&"),buffer)){}
      else if(try_push(r,gbstd::string_view("&="),buffer)){}
      else if(try_push(r,gbstd::string_view("&"),buffer)){}
      else if(try_push(r,gbstd::string_view("^="),buffer)){}
      else if(try_push(r,gbstd::string_view("^"),buffer)){}
      else if(try_push(r,gbstd::string_view("=="),buffer)){}
      else if(try_push(r,gbstd::string_view("="),buffer)){}
      else if(try_push(r,gbstd::string_view("!="),buffer)){}
      else if(try_push(r,gbstd::string_view("!"),buffer)){}
      else if(try_push(r,gbstd::string_view("::"),buffer)){}
      else if(try_push(r,gbstd::string_view(":"),buffer)){}
      else if(try_push(r,gbstd::string_view(","),buffer)){}
      else if(try_push(r,gbstd::string_view("?"),buffer)){}
      else
        if(r.is_pointing_identifier())
        {
          buffer.emplace_back(identifier(r.read_identifier()));
        }

      else
        if(r.is_pointing_number())
        {
          buffer.emplace_back(r.read_number());
        }

      else
        {
          printf("処理できない文字です %d\n",c);

          break;
        }


      buffer.back().set_pointer(p);
    }


  m_length = buffer.size();

  m_data = new token[m_length];

  token*  p = m_data;

    for(auto&&  etok: buffer)
    {
      *p++ = std::move(etok);
    }
}




token_string&
token_string::
operator=(const token_string&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_data = new token[rhs.m_length];

      m_length = rhs.m_length;
      m_open   = rhs.m_open  ;
      m_close  = rhs.m_close ;

      std::copy(rhs.begin(),rhs.end(),m_data);
    }


  return *this;
}


token_string&
token_string::
operator=(token_string&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data  ,rhs.m_data  );
      std::swap(m_length,rhs.m_length);
      std::swap(m_open  ,rhs.m_open  );
      std::swap(m_close ,rhs.m_close );
    }


  return *this;
}



void
token_string::
clear() noexcept
{
  delete[] m_data          ;
           m_data = nullptr;

  m_length = 0;

  m_open  = 0;
  m_close = 0;
}


const token*  token_string::begin() const noexcept{return m_data;}
const token*    token_string::end() const noexcept{return m_data+m_length;}


void
token_string::
print(FILE*  f, int  indent) const noexcept
{
    if(m_open)
    {
      fprintf(f,"%c",m_open);
    }


  auto  it     = begin();
  auto  it_end =   end();

    if(it != it_end)
    {
      it++->print(f,indent);

        while(it != it_end)
        {
          it++->print(f,indent);
        }
    }


    if(m_close)
    {
      fprintf(f,"%c",m_close);
    }
}


}}




