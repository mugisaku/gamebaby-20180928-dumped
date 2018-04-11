#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
menu::
do_when_mouse_acted(int  x, int  y) noexcept
{
  x /= m_parameter.item_width ;
  y /= m_parameter.item_height;

    if(m_parameter.item_reactor(point(x,y)))
    {
      need_to_redraw();
    }
}


void
menu::
reform(point  base_pt) noexcept
{
  m_width  = m_parameter.item_width *m_parameter.table_width ;
  m_height = m_parameter.item_height*m_parameter.table_height;

  widget::reform(base_pt);
}


void
menu::
render(image_cursor  cur) noexcept
{
    for(int  y = 0;  y < m_parameter.table_height;  ++y){
    for(int  x = 0;  x < m_parameter.table_width ;  ++x){
      auto  pt = point(m_parameter.item_width*x,m_parameter.item_height*y);

      m_parameter.item_renderer(cur+pt,point(x,y));
    }}
}


}}




