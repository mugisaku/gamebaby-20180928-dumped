#include"libgbstd/text.hpp"




namespace gbstd{
namespace texts{


gbstd::u16string_view
text_roll::iterator::
operator*() const noexcept
{
  size_t  len = (m_line->current-m_line->begin);

  return gbstd::u16string_view(m_line->begin,len);
}


bool
text_roll::iterator::
operator!=(const iterator&  rhs) const noexcept
{
  return m_line != rhs.m_line;
}


text_roll::iterator&
text_roll::iterator::
operator++() noexcept
{
  m_line = m_line->next;

  return *this;
}


}}




