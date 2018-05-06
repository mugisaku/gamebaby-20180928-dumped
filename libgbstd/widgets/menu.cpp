#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
menu::
set_item_size(int  w, int  h) noexcept
{
  m_parameter.width  = w;
  m_parameter.height = h;

  need_to_reform();
}


void
menu::
update() noexcept
{
  auto  mouse = get_mouse();

  mouse->x /= m_parameter.width ;
  mouse->y /= m_parameter.height;

  m_parameter.reactor(*this,mouse.point,mouse.left_button,mouse.right_button);
}


void
menu::
reform(point  base_pt) noexcept
{
  m_width  = m_parameter.width *m_table_width ;
  m_height = m_parameter.height*m_table_height;

  widget::reform(base_pt);
}


void
menu::
render(image_cursor  cur) noexcept
{
  widget::render_background(cur);

    for(int  y = 0;  y < m_table_height;  ++y){
    for(int  x = 0;  x < m_table_width ;  ++x){
      auto  pt = point(m_parameter.width*x,m_parameter.height*y);

      m_parameter.renderer(*this,point(x,y),cur+pt);
    }}
}


}}




