#include"libgbscr/token.hpp"
#include<new>


namespace  gbscr{
namespace tokens{




token&
token::
operator=(uint64_t  i) noexcept
{
  clear();

  m_kind = kind::integer_literal;

  m_data.i = i;

  return *this;
}


token&
token::
operator=(const identifier&  id) noexcept
{
  clear();

  m_kind = kind::identifier;

  new(&m_data) identifier(id);

  return *this;
}


token&
token::
operator=(operator_word  opw) noexcept
{
  clear();

  m_kind = kind::operator_word;

  m_data.opw = opw;

  return *this;
}


token&
token::
operator=(token_string&&  toks) noexcept
{
  clear();

  m_kind = kind::token_string;

  new(&m_data) token_string(std::move(toks));

  return *this;
}


token&
token::
operator=(semicolon  semcol) noexcept
{
  clear();

  m_kind = kind::semicolon;

  return *this;
}


token&
token::
operator=(const token&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;
      m_stream_context = rhs.m_stream_context;

        switch(m_kind)
        {
      case(kind::integer_literal):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::identifier):
          new(&m_data) identifier(rhs.m_data.id);
          break;
      case(kind::operator_word):
          m_data.opw = rhs.m_data.opw;
          break;
      case(kind::token_string):
          new(&m_data) token_string(rhs.m_data.toks);
          break;
        }
    }


  return *this;
}


token&
token::
operator=(token&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);
      std::swap(m_stream_context,rhs.m_stream_context);

        switch(m_kind)
        {
      case(kind::integer_literal):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::identifier):
          new(&m_data) identifier(std::move(rhs.m_data.id));
          break;
      case(kind::operator_word):
          m_data.opw = rhs.m_data.opw;
          break;
      case(kind::token_string):
          new(&m_data) token_string(std::move(rhs.m_data.toks));
          break;
        }
    }


  return *this;
}



void
token::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::integer_literal):
      break;
  case(kind::identifier):
      gbstd::destruct(m_data.id);
      break;
  case(kind::operator_word):
      break;
  case(kind::token_string):
      gbstd::destruct(m_data.toks);
      break;
    }


  m_kind = kind::null;
}




bool
token::
is_identifier(string_view_list  ls) const noexcept
{
    if(is_identifier())
    {
      auto  idsv = m_data.id.view();

        for(auto&  sv: ls)
        {
            if(idsv == sv)
            {
              return true;
            }
        }
    }


  return false;
}


bool
token::
is_operator_word(string_view_list  ls) const noexcept
{
    if(is_operator_word())
    {
        for(auto&  sv: ls)
        {
            if(m_data.opw == operator_word(sv))
            {
              return true;
            }
        }
    }


  return false;
}


bool
token::
is_token_string(char  open, char  close) const noexcept
{
    if(is_token_string())
    {
      auto&  toks = get_token_string();

      return(toks.get_open()  ==  open) &&
            (toks.get_close() == close);
    }


  return false;
}




void
token::
print(FILE*  f, int  indent) const noexcept
{
    switch(m_kind)
    {
  case(kind::integer_literal):
      fprintf(f,"%lu",m_data.i);
      break;
  case(kind::identifier):
      fprintf(f,"%s ",m_data.id.data());
      break;
  case(kind::operator_word):
      fprintf(f,"%s ",m_data.opw.data());
      break;
  case(kind::token_string):
      m_data.toks.print(f,indent);
      break;
  case(kind::semicolon):
      fprintf(f,";");
      break;
    }
}


}}




