#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"




void
search_route(const piece&  p, point  a, point  b) noexcept
{
}



void
reset_board() noexcept
{
    for(int  y = 0;  y < g_board_height;  ++y){
    for(int  x = 0;  x < g_board_width ;  ++x){
      auto&  sq = g_board.get_square(x,y);

      sq.set_index(point(x,y));

/*
      bool  u = (y               );
      bool  l = (x               );
      bool  r = (x < (m_width -1));
      bool  d = (y < (m_height-1));

      sq.link(u? &get_square(x  ,y-1):&get_square(x  ,h-1),links::up   );
      sq.link(l? &get_square(x-1,y  ):&get_square(w-1,y  ),links::left );
      sq.link(r? &get_square(x+1,y  ):&get_square(  0,y  ),links::right);
      sq.link(d? &get_square(x  ,y+1):&get_square(x  ,0  ),links::down );

      sq.link((u && l)? &get_square(x-1,y-1):&get_square(w-1,h-1),links::upper_left );
      sq.link((u && r)? &get_square(x+1,y-1):&get_square(0  ,h-1),links::upper_right);
      sq.link((d && l)? &get_square(x-1,y+1):&get_square(w-1,  0),links::lower_left );
      sq.link((d && r)? &get_square(x+1,y+1):&get_square(0  ,  0),links::lower_right);
*/
    }}
}


void
clean_board() noexcept
{
    for(int  y = 0;  y < g_board_height;  ++y){
    for(int  x = 0;  x < g_board_width ;  ++x){
      auto&  sq = g_board.get_square(x,y);

      sq.set_distance(0);

      sq.set_mv_consumption(-1);

      sq.set_mv(0);
    }}
}


basic_board<square,g_board_width,g_board_height>
g_board;


basic_board_view<square,g_board_width,g_board_height>
g_board_view;





