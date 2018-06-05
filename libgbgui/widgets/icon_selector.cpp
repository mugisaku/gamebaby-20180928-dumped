#include"libgbgui/widget.hpp"



namespace gbgui{
namespace widgets{




void
icon_selector::
render(image_cursor  cur) noexcept
{
  auto&  icon = *m_icons[m_current];

    for(auto  y = 0;  y < icon::size;  ++y){
    for(auto  x = 0;  x < icon::size;  ++x){
      cur.draw_dot(icon.get_color(x,y),x,y);
    }}
}




}}




