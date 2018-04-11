#include"libgbstd/text.hpp"




namespace gbstd{
namespace texts{




void
text::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_chars.resize(w*h);
}


void
text::
fill_line(char16_t  c, int  y) noexcept
{
  auto  p = &get_char(0,y);

  int  n = m_width;

    while(n--)
    {
      *p++ = c;
    }
}


}}




