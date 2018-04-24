#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




window::
window(gbstd::string_view  name) noexcept:
m_name(name),
m_root(*this)
{
}




bool
window::
test_by_point(int  x, int  y) const noexcept
{
  return((x >= (m_point.x             )) &&
         (y >= (m_point.y             )) &&
         (x <  (m_point.x+get_width() )) &&
         (y <  (m_point.y+get_height())));
}


void
window::
change_state(uint32_t  st) noexcept
{
  m_state = st;

  m_root->need_to_reform();

  need_to_redraw_frame();
}


void
window::
react() noexcept
{
  m_root.react(m_point);

  update();
}


bool
window::
update() noexcept
{
    if(m_root->is_needed_to_reform())
    {
      m_root->reform(point());


      int  w = m_root->get_width() ;
      int  h = m_root->get_height();

        if((w != get_width()) ||
           (h != get_height()))
        {
          get_image().resize(w,h);
        }


      m_root.redraw(m_image);

      return true;
    }

  else
    {
      return m_root.redraw_only_needed_widgets(m_image);
    }
}


void
window::
redraw(const window_style&  style, image&  dst) noexcept
{
    if(test_flag(flags::needed_to_redraw_frame))
    {
      draw_frame(style,image_cursor(dst,m_point));

      images::transfer(m_image,point(),0,0,image_cursor(dst,m_point));
    }
}




}}




