#include"libgbscr/token.hpp"
#include"libgbscr/stream.hpp"
#include<vector>


namespace  gbscr{
namespace tokens{




token_string::
token_string(stream&  s, char  open, char  close):
m_open(open),
m_close(close)
{
  std::vector<token>  buffer;

    for(;;)
    {
      s.skip_spaces();

      auto  c = s.get_char();

        if(c == close)
        {
          s.advance();

          break;
        }

      else
        {
          auto&  tok = s.read_token();

            if(tok)
            {
              buffer.emplace_back(tok);
            }

          else
            {
              break;
            }
        }
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




