#include"libgbsnd/script.hpp"


namespace gbsnd{
namespace scripts{


const script_token
script_token_cursor::
null;


const script_token&
script_token_cursor::
operator*() const noexcept
{
  return (*this)? *m_current:null;
}


const script_token*
script_token_cursor::
operator->() const noexcept
{
  return (*this)? m_current:&null;
}


const script_token&
script_token_cursor::
operator[](int  i) const noexcept
{
  return ((m_current+i) < m_end)? m_current[i]:null;
}



script_token_cursor&
script_token_cursor::
operator+=(int  n) noexcept
{
  m_current += n;

  return *this;
}


script_token_cursor&
script_token_cursor::
operator++() noexcept
{
  ++m_current;

  return *this;
}


script_token_cursor
script_token_cursor::
operator++(int  n) noexcept
{
  auto  cur(*this);

  ++m_current;

  return cur;
}


}}




