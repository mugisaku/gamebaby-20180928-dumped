#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




text_roll::
text_roll(int  w, int  h) noexcept:
m_text(w,h)
{
}




void
text_roll::
clear() noexcept
{
  m_queue.clear();

  m_text.fill(0);

  m_start_line = 0;

  m_cursor = point();

  m_full_lined = false;

  need_to_redraw();
}


void
text_roll::
linefeed() noexcept
{
  m_cursor.x = 0;

    if(++m_cursor.y >= m_text.get_height())
    {
      m_cursor.y = 0;

      m_full_lined = true;
    }


    if(m_full_lined)
    {
        if(++m_start_line >= m_text.get_height())
        {
          m_start_line = 0;
        }
    }

  need_to_redraw();
}


void
text_roll::
pump() noexcept
{
    if(m_queue)
    {
      m_text.get_char(m_cursor.x++,m_cursor.y) = m_queue.pop();

      need_to_redraw();
    }
}


void
text_roll::
reform(point  base_pt) noexcept
{
  m_width  = font_width *m_text.get_width() ;
  m_height = font_height*m_text.get_height();

  widget::reform(base_pt);
}


void
text_roll::
render(image_cursor  cur) noexcept
{
  int  w = m_text.get_width();
  int  h = m_text.get_height();
  int  n = h;

  int  l = m_start_line;

  int  y = 0;

    while(n--)
    {
      cur.draw_text(gbstd::u16string_view(&m_text.get_char(0,l++),w),text_style(),0,y);

        if(l >= h)
        {
          l = 0;
        }


      y += font_height;
    }
}


}}




