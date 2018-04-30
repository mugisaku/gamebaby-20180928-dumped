#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




window::
window(gbstd::string_view  name, widget*  widget) noexcept:
m_name(name),
m_root(*this,widget)
{
  m_root.put_down();

  m_root.get_node().show_all();
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
react(const control_device&  condev) noexcept
{
  update();

  m_root.react(condev);

  update();
}


void
window::
update() noexcept
{
    if(has_header())
    {
      m_root.set_offset(m_point+point(8,16));
    }

  else
    {
      m_root.set_offset(m_point);
    }


    if(m_root.get_node().is_needed_to_reform())
    {
      m_root.get_node().reform(point());


      int  w = m_root.get_node().get_width() ;
      int  h = m_root.get_node().get_height();

        if((w != m_content_image.get_width()) ||
           (h != m_content_image.get_height()))
        {
          m_content_image.resize(w,h);

          m_width  = w+16;
          m_height = h+16+(has_header()? 8:0);


          set_flag(flags::needed_to_update_frame);
        }


      m_root.redraw(m_content_image);

      need_to_redraw();
    }

  else
    {
        if(m_root.redraw_only_needed_widgets(m_content_image))
        {
          need_to_redraw();
        }
    }
}


void
window::
redraw_content(image&  dst) noexcept
{
  auto  cur = image_cursor(dst,m_root.get_offset());

  images::transfer(m_content_image,m_content_image.get_rectangle(),cur);

  unset_flag(flags::needed_to_redraw);
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


void
window::
print() const noexcept
{
  printf("window %s{...}\n",m_name.data());
}




}}




