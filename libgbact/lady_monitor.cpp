#include"libgbact/character.hpp"




namespace gbact{
namespace characters{




lady_monitor::
lady_monitor(lady&  lady, int  x, int  y) noexcept:
m_target(&lady)
{
  set_base_point(x,y);
}




void
lady_monitor::
render(point  offset, images::image&  dst) noexcept
{
  auto  l = m_target->get_life_level();

  point  pt = (l == 0)? point(48*5,48*2)
             :(l == 1)? point(48*4,48*3)
             :(l == 2)? point(48*3,48*3)
             :(l == 3)? point(48*2,48*3)
             :(l == 4)? point(48*1,48*3)
             :          point(48*0,48*3);

  images::paste(g_image,rectangle(pt.x,pt.y,48,48),dst,get_base_point());
}




}}




