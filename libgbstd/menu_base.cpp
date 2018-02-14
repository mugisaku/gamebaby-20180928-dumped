#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




void
menu_base::
render(image&  dst, images::point  dst_point, int  start_index, int  number_of_columns) const noexcept
{
  int  current_index = start_index;

    for(int  y = 0;  y < m_number_of_visible_rows;  ++y){
    for(int  x = 0;  x <        number_of_columns;  ++x){
      m_renderer(dst,dst_point,point(x,y),current_index++);
    }}
}




}}




