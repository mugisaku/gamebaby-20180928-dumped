#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"




void
search_route(const piece&  p, point  a, point  b) noexcept
{
    for(int  y = 0;  y < g_board_height;  ++y){
    for(int  x = 0;  x < g_board_width ;  ++x){
      auto&  sq = g_board.get_square(x,y);

      sq.clear_distance();

      sq.set_mv(0);
    }}


  auto&  start = g_board.get_square(a.x,a.y);
  auto&   goal = g_board.get_square(b.x,b.y);

  int  index = 0;

  static std::vector<square*>  routing_stack;

  routing_stack.clear();

  start.set_mv(p.get_mv()-p.get_mv_consumption(start));

  routing_stack.emplace_back(&start);

    while(index < routing_stack.size())
    {
      auto&  sq = *(routing_stack[index++]);

      auto  mv = sq.get_mv();

        if(mv > 0)
        {
          constexpr point  offset_table[] = {
            point(-1,-1),
            point( 0,-1),
            point( 1,-1),
            point(-1, 0),
            point( 1, 0),
            point(-1, 1),
            point( 0, 1),
            point( 1, 1),
          };

            for(auto&  off: offset_table)
            {
              auto  i = sq.get_index()+off;

                if(g_board.test_index(i))
                {
                  auto&  dst = g_board.get_square(i.x,i.y);

                    if(!dst.test_mark())
                    {
                      dst.set_mv(mv-p.get_mv_consumption(dst));

                      routing_stack.emplace_back(&dst);
                    }
                }
            }
        }
    }
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


basic_board<square,g_board_width,g_board_height>
g_board;


basic_board_view<square,g_board_width,g_board_height>
g_board_view;





