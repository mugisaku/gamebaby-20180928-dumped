#include"libgbgui/widget.hpp"



namespace gbgui{
namespace widgets{




text_roll::
text_roll(int  w, int  h) noexcept:
m_text(w,h)
{
  m_style.set_color(0,colors::blue);
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
      m_text.fill_line(0,m_cursor.y);

      m_needing_to_fill = true;

        if(++m_start_line >= m_text.get_height())
        {
          m_start_line = 0;
        }
    }


  need_to_redraw();
}


void
text_roll::
type() noexcept
{
    if(m_queue)
    {
      auto  c = m_queue.pop();

        if(c)
        {
          m_text.get_char(m_cursor.x++,m_cursor.y) = c;

          need_to_redraw();
        }
    }

  else
    if(m_queue[0])
    {
      m_queue.clear();
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
    if(m_needing_to_fill)
    {
      cur.fill_rectangle(m_style.get_color(0),0,0,m_width,m_height);

      m_needing_to_fill = false;
    }


  int  w = m_text.get_width();
  int  h = m_text.get_height();
  int  n = h;

  int  l = m_start_line;

  int  y = 0;

    while(n--)
    {
      cur.draw_text(gbstd::u16string_view(&m_text.get_char(0,l++),w),m_style,0,y);

        if(l >= h)
        {
          l = 0;
        }


      y += font_height;
    }
}


}}




