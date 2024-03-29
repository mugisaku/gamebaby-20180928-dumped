#include"libgbgui/widget.hpp"



namespace gbgui{
namespace widgets{




background_style
widget::
m_default_background_style;




void
widget::
set_userdata(void*  data, std::initializer_list<widget*>  ls) noexcept
{
    for(auto  w: ls)
    {
      w->set_userdata(data);
    }
}


void
widget::
set_style(const background_style&  new_style) noexcept
{
  m_background_style = &new_style;

  need_to_redraw();
}


void
widget::
notify_flag(uint32_t  v) noexcept
{
  set_flag(v);

    if(m_parent)
    {
      m_parent->notify_flag(v);
    }
}


mouse
widget::
get_mouse() const noexcept
{
    if(!m_root)
    {
      printf("widget::get_mouse error\n");

      static mouse  dummy;

      return dummy;
    }


  auto  m = m_root->get_mouse();

  m.point -= m_absolute_point;

  return m;
}


void
widget::
need_to_redraw() noexcept
{
    if(!m_root)
    {
//      printf("widget::need_to_redraw error: %s::%s rootがない\n",get_widget_name(),m_name.data());

      return;
    }


    if(!is_needed_to_redraw())
    {
      set_flag(flags::needed_to_redraw);

      m_root->push_widget_that_needed_to_redraw(*this);
    }
}


void
widget::
need_to_reform() noexcept
{
  notify_flag(flags::needed_to_reform);
}


bool
widget::
test_by_absolute_point(point  pt) const noexcept
{
  return((pt.x >= m_absolute_point.x) &&
         (pt.y >= m_absolute_point.y) &&
         (pt.x <  (m_absolute_point.x+m_width )) &&
         (pt.y <  (m_absolute_point.y+m_height)));
}


void
widget::
reform(point  base_pt) noexcept
{
  m_absolute_point = base_pt+m_relative_point;

  unset_flag(flags::needed_to_reform);

  need_to_redraw();
}


void
widget::
redraw(image&  img) noexcept
{
  render(image_cursor(img,m_absolute_point));

  unset_flag(flags::needed_to_redraw);
}


void
widget::
reform_if_needed(point  base_pt) noexcept
{
    if(test_flag(flags::needed_to_reform))
    {
      reform(base_pt);
    }
}



void
widget::
render_background(image_cursor  cur) const noexcept
{
  int  w = m_width;
  int  h = m_height;

  auto&  bgst = *m_background_style;

    if(bgst.is_single_color())
    {
      cur.fill_rectangle(bgst.get_first_color(),0,0,w,h);
    }

  else
    if(bgst.is_stripe())
    {
      cur.draw_stripe_rectangle(bgst.get_first_color(),bgst.get_second_color(),bgst.get_interval(),0,0,w,h);
    }
}


void
widget::
print(int  indent) const noexcept
{
  printf("%p %s(%s) - flags:%d w:%4d h:%4d rel_pt{%4d,%4d} abs_pt{%4d,%4d}",this,m_name.data(),get_widget_name(),
    m_flags,
    m_width,
    m_height,
    m_relative_point.x,
    m_relative_point.y,
    m_absolute_point.x,
    m_absolute_point.y
  );
}


}}




