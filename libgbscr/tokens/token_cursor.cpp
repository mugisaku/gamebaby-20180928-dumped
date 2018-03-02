#include"libgbscr/token.hpp"


namespace  gbscr{
namespace tokens{


const token
token_cursor::
null;


const token&
token_cursor::
operator*() const noexcept
{
  return (*this)? *m_current:null;
}


const token*
token_cursor::
operator->() const noexcept
{
  return (*this)? m_current:&null;
}


const token&
token_cursor::
operator[](int  i) const noexcept
{
  return ((m_current+i) < m_end)? m_current[i]:null;
}



token_cursor&
token_cursor::
operator+=(int  n) noexcept
{
  m_current += n;

  return *this;
}


token_cursor&
token_cursor::
operator++() noexcept
{
  ++m_current;

  return *this;
}


token_cursor
token_cursor::
operator++(int  n) noexcept
{
  auto  cur(*this);

  ++m_current;

  return cur;
}


}}




