#include"libgbscr/token.hpp"
#include<new>


namespace  gbscr{
namespace tokens{




void
token::
set_data(uint64_t  i) noexcept
{
  unset_data();

  m_kind = kind::integer_literal;

  m_data.i = i;
}


void
token::
set_data(const shared_string&  s) noexcept
{
  unset_data();

  m_kind = kind::string_literal;

  new(&m_data) shared_string(s);
}


void
token::
set_data(const identifier&  id) noexcept
{
  unset_data();

  m_kind = kind::identifier;

  new(&m_data) shared_string(*id);
}


void
token::
set_data(operator_word  opw) noexcept
{
  unset_data();

  m_kind = kind::operator_word;

  m_data.opw = opw;
}


void
token::
set_data(token_string&&  toks) noexcept
{
  unset_data();

  m_kind = kind::token_string;

  new(&m_data) token_string(std::move(toks));
}


void
token::
set_data(semicolon  semcol) noexcept
{
  unset_data();

  m_kind = kind::semicolon;
}


token&
token::
operator=(const token&  rhs) noexcept
{
    if(this != &rhs)
    {
      unset_data();

      m_kind = rhs.m_kind;
      m_pointer = rhs.m_pointer;

        switch(m_kind)
        {
      case(kind::integer_literal):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::string_literal):
      case(kind::identifier):
          new(&m_data) shared_string(rhs.m_data.s);
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
      unset_data();

      std::swap(m_kind,rhs.m_kind);
      std::swap(m_pointer,rhs.m_pointer);

        switch(m_kind)
        {
      case(kind::integer_literal):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::string_literal):
      case(kind::identifier):
          new(&m_data) shared_string(std::move(rhs.m_data.s));
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
unset_data() noexcept
{
    switch(m_kind)
    {
  case(kind::integer_literal):
      break;
  case(kind::string_literal):
  case(kind::identifier):
      gbstd::destruct(m_data.s);
      break;
  case(kind::operator_word):
      break;
  case(kind::token_string):
      gbstd::destruct(m_data.toks);
      break;
    }


  m_kind = kind::null;
}


void
token::
clear() noexcept
{
  unset_data();

  m_pointer = nullptr;
}




bool
token::
is_identifier(string_view_list  ls) const noexcept
{
    if(is_identifier())
    {
      auto  idsv = m_data.s.view();

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
  short_string  ss;

    switch(m_kind)
    {
  case(kind::integer_literal):
      fprintf(f,"%lu",m_data.i);
      break;
  case(kind::string_literal):
      fprintf(f,"\"%s\"",m_data.s.data());
      break;
  case(kind::identifier):
      fprintf(f,"%s ",m_data.s.data());
      break;
  case(kind::operator_word):
      ss = m_data.opw;

      fprintf(f,"%s ",ss.data());
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




