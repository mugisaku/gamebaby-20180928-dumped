#include"libgbstd/board.hpp"




namespace gbstd{
namespace boards{




void
board::
build(int  w, int  h, int  square_size, square_data*  (*get)(int  x, int  y)) noexcept
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

      spaces::area  area;

      area.top    = y*square_size;
      area.left   = x*square_size;
      area.right  = area.left+square_size-1;
      area.bottom = area.top +square_size-1;

      sq.set_area(area);

      sq.set_index(point(x,y));

        if(get)
        {
          sq.set_data(get(x,y));
        }


      bool  u = (y               );
      bool  l = (x               );
      bool  r = (x < (m_width -1));
      bool  d = (y < (m_height-1));

      sq.set_link(u? &get_square(x  ,y-1):nullptr,links::up   );
      sq.set_link(l? &get_square(x-1,y  ):nullptr,links::left );
      sq.set_link(r? &get_square(x+1,y  ):nullptr,links::right);
      sq.set_link(d? &get_square(x  ,y+1):nullptr,links::down );

      sq.set_link((u && l)? &get_square(x-1,y-1):nullptr,links::upper_left );
      sq.set_link((u && r)? &get_square(x+1,y-1):nullptr,links::upper_right);
      sq.set_link((d && l)? &get_square(x-1,y+1):nullptr,links::lower_left );
      sq.set_link((d && r)? &get_square(x+1,y+1):nullptr,links::lower_right);
    }}
}




}}




