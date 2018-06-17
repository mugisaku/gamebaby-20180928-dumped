#include"libgbstd/board.hpp"
#include"libgbstd/space.hpp"




namespace gbstd{
namespace boards{




void
board::
build(int  w, int  h, int  square_size, square_data&  default_data) noexcept
{
  m_width  = w;
  m_height = h;

  m_square_size = square_size;

  m_image_width  = square_size*w;
  m_image_height = square_size*h;

  m_square_table.resize(w*h);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  sq = get_square(x,y);

      areas::area  area;

      area.top    = y*square_size;
      area.left   = x*square_size;
      area.right  = area.left+square_size-1;
      area.bottom = area.top +square_size-1;

      sq.set_area(area);

      sq.set_data(default_data);

      sq.set_index(point(x,y));


      bool  u = (y               );
      bool  l = (x               );
      bool  r = (x < (m_width -1));
      bool  d = (y < (m_height-1));

      sq.set_link(u? &get_square(x  ,y-1):&get_square(x  ,h-1),links::up   );
      sq.set_link(l? &get_square(x-1,y  ):&get_square(w-1,y  ),links::left );
      sq.set_link(r? &get_square(x+1,y  ):&get_square(  0,y  ),links::right);
      sq.set_link(d? &get_square(x  ,y+1):&get_square(x  ,0  ),links::down );

      sq.set_link((u && l)? &get_square(x-1,y-1):&get_square(w-1,h-1),links::upper_left );
      sq.set_link((u && r)? &get_square(x+1,y-1):&get_square(0  ,h-1),links::upper_right);
      sq.set_link((d && l)? &get_square(x-1,y+1):&get_square(w-1,  0),links::lower_left );
      sq.set_link((d && r)? &get_square(x+1,y+1):&get_square(0  ,  0),links::lower_right);
    }}
}




square&
board::
get_square_by_object(const spaces::object&  o) noexcept
{
  auto  x = (static_cast<int>(o.get_base_point().x)%m_image_width );
  auto  y = (static_cast<int>(o.get_base_point().y)%m_image_height);

    if(x < 0)
    {
      x += m_image_width;
    }


    if(y < 0)
    {
      y += m_image_height;
    }


  return get_square(x/m_square_size,y/m_square_size);
}


const square&
board::
get_square_by_object(const spaces::object&  o) const noexcept
{
  auto  x = (static_cast<int>(o.get_base_point().x)%m_image_width );
  auto  y = (static_cast<int>(o.get_base_point().y)%m_image_height);

    if(x < 0)
    {
      x += m_image_width;
    }


    if(y < 0)
    {
      y += m_image_height;
    }


  return get_square(x/m_square_size,y/m_square_size);
}




void
board::
put_to_around(square_data&  sqdat) noexcept
{
    for(int  x = 0;  x < m_width;  ++x)
    {
      get_square(x,         0).set_data(sqdat);
      get_square(x,m_height-1).set_data(sqdat);
    }


    for(int  y = 1;  y < (m_height-1);  ++y)
    {
      get_square(        0,y).set_data(sqdat);
      get_square(m_width-1,y).set_data(sqdat);
    }
}


}}




