#include"libgbscr/token.hpp"


namespace  gbscr{
namespace tokens{




token&
token::
operator=(token&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unset_data();

      std::swap(m_kind   ,rhs.m_kind   );
      std::swap(m_pointer,rhs.m_pointer);

        switch(m_kind)
        {
      case(token_kind::binary_integer):
      case(token_kind::octal_integer):
      case(token_kind::decimal_integer):
      case(token_kind::hexadecimal_integer):
      case(token_kind::string):
      case(token_kind::identifier):
      case(token_kind::punctuations):
          new(&m_data) gbstd::string(rhs.m_data.str);
          break;
      case(token_kind::block):
          m_data.blk = rhs.m_data.blk;
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
  case(token_kind::binary_integer):
  case(token_kind::octal_integer):
  case(token_kind::decimal_integer):
  case(token_kind::hexadecimal_integer):
  case(token_kind::string):
  case(token_kind::identifier):
  case(token_kind::punctuations):
      gbstd::destruct(m_data.str);
      break;
  case(token_kind::block):
      delete m_data.blk;
      break;
    }


  m_kind = token_kind::null;
}


void
token::
set_data(token_kind  k, gbstd::string&&  s) noexcept
{
  unset_data();

  m_kind = k;

  new(&m_data) gbstd::string(std::move(s));
}


void
token::
set_data(block&  blk) noexcept
{
  unset_data();

  m_kind = token_kind::block;

  m_data.blk = &blk;
}




bool
token::
is_block(char  open, char  close) const noexcept
{
  return is_block() && m_data.blk->test(open,close);
}


void
token::
print(int  indent) const noexcept
{
    switch(m_kind)
    {
  case(token_kind::binary_integer):
      printf("0b%s ",m_data.str.data());
      break;
  case(token_kind::octal_integer):
      printf("0o%s ",m_data.str.data());
      break;
  case(token_kind::decimal_integer):
      printf("%s ",m_data.str.data());
      break;
  case(token_kind::hexadecimal_integer):
      printf("0x%s ",m_data.str.data());
      break;
  case(token_kind::string):
      printf("\"%s\"",m_data.str.data());
      break;
  case(token_kind::identifier):
      printf("%s ",m_data.str.data());
      break;
  case(token_kind::punctuations):
      printf("%s ",m_data.str.data());
      break;
  case(token_kind::block):
      m_data.blk->print(indent+1);
      break;
    }
}



}}




