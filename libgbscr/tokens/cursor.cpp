#include"libgbscr/token.hpp"


namespace  gbscr{
namespace tokens{


const token
cursor::
null;


const token&
cursor::
operator*() const noexcept
{
  return (*this)? *m_current:null;
}


const token*
cursor::
operator->() const noexcept
{
  return (*this)? m_current:&null;
}


const token&
cursor::
operator[](int  i) const noexcept
{
  return ((m_current+i) < m_end)? m_current[i]:null;
}



cursor&
cursor::
operator+=(int  n) noexcept
{
  m_current += n;

  return *this;
}


cursor&
cursor::
operator++() noexcept
{
  ++m_current;

  return *this;
}


cursor
cursor::
operator++(int  n) noexcept
{
  auto  cur(*this);

  ++m_current;

  return cur;
}




}}




