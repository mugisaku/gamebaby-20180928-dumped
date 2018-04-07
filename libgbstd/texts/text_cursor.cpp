#include"libgbstd/text.hpp"




namespace gbstd{
namespace texts{




void
text_cursor::
advance() noexcept
{
    if(++m_point.x >= m_text->get_width())
    {
      m_point.x = 0;

        if(++m_point.y >= m_text->get_height())
        {
          m_point.y = 0;
        }
    }
}


void
text_cursor::
fill_line(char16_t  c) const noexcept
{
  int  x = m_point.x;
  int  y = m_point.y;

    while(x < m_text->get_width())
    {
      m_text->get_char(x++,y) = c;
    }
}




}}




