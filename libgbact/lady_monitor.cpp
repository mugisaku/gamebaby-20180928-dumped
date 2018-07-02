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
render(point  offset, image_cursor  cur) noexcept
{
  auto  l = m_target->get_life_level();

  point  pt = m_target->is_rejoicing()? point(48*5,48*3)
             :m_target->is_crying()?    point(48*5,48*2)
             :(l == 0)? point(48*3,48*3)
             :(l == 1)? point(48*2,48*3)
             :(l == 2)? point(48*1,48*3)
             :(l == 3)? point(48*0,48*3)
             :          point(48*0,48*3);

  images::paste(g_image,rectangle(pt.x,pt.y,48,48),cur+(offset+get_base_point()));

  int  x = 48;

    while(l--)
    {
      images::paste(g_image,rectangle(48*5+24,24,24,24),cur+(offset+get_base_point()+point(x,24)));

       x += 24;
    }
}




}}




