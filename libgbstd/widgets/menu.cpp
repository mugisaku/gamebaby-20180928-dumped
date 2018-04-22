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
do_when_mouse_acted(int  x, int  y) noexcept
{
  x /= m_parameter.width ;
  y /= m_parameter.height;

    if(m_parameter.reactor(point(x,y)))
    {
      need_to_redraw();
    }
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
    for(int  y = 0;  y < m_table_height;  ++y){
    for(int  x = 0;  x < m_table_width ;  ++x){
      auto  pt = point(m_parameter.width*x,m_parameter.height*y);

      m_parameter.renderer(cur+pt,point(x,y));
    }}
}


}}




