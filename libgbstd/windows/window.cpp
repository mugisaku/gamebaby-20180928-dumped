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
  return((x >= (m_point.x         )) &&
         (y >= (m_point.y         )) &&
         (x <  (m_point.x+m_width )) &&
         (y <  (m_point.y+m_height)));
}


void
window::
react() noexcept
{
  update();

  m_root.react(m_content_point);

  update();
}


bool
window::
update() noexcept
{
  m_content_point = m_point;

    if(has_header())
    {
      m_content_point += point(8,16);
    }


    if(m_root->is_needed_to_reform())
    {
      m_root->reform(point());


      int  w = m_root->get_width() ;
      int  h = m_root->get_height();

        if((w != m_content_image.get_width()) ||
           (h != m_content_image.get_height()))
        {
          m_content_image.resize(w,h);

          m_width  = w+16;
          m_height = h+16+(has_header()? 8:0);


          set_flag(flags::needed_to_update_frame);
        }


      m_root.redraw(m_content_image);

      return true;
    }

  else
    {
 m_root.redraw_only_needed_widgets(m_content_image);
/*
      m_root.redraw(m_content_image);
*/
      return true;
    }
}


void
window::
redraw_content(image&  dst) const noexcept
{
  auto  cur = image_cursor(dst,m_content_point);

  images::transfer(m_content_image,m_content_image.get_rectangle(),cur);
}


void
window::
redraw_frame(image&  dst) noexcept
{
    if(test_flag(flags::needed_to_update_frame))
    {
        if((m_frame_image.get_width()  != m_width ) ||
           (m_frame_image.get_height() != m_height))
        {
          m_frame_image.resize(m_width,m_height);
        }


      update_frame_image();

      unset_flag(flags::needed_to_update_frame);
    }


  auto  cur = image_cursor(dst,m_point);

  images::transfer(m_frame_image,m_frame_image.get_rectangle(),cur);
}




}}




