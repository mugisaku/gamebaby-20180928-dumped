#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"




image
g_bg_image;




route
search_route(const square&  start, const square&  goal) noexcept
{
  auto  ptr = &goal;

  route  r;

    for(;;)
    {
      constexpr std::pair<point,instruction>  table[] = {
        std::make_pair(point(-1,-1),instruction::movdr),
        std::make_pair(point( 0,-1),instruction::movd),
        std::make_pair(point( 1,-1),instruction::movdl),
        std::make_pair(point(-1, 0),instruction::movr),
        std::make_pair(point( 1, 0),instruction::movl),
        std::make_pair(point(-1, 1),instruction::movur),
        std::make_pair(point( 0, 1),instruction::movu),
        std::make_pair(point( 1, 1),instruction::movul),
      };


      auto  last_ptr = ptr;

      instruction  instr;

        for(auto&  e: table)
        {
          auto  dst_i = last_ptr->get_index()+e.first;

            if(g_board.test_index(dst_i))
            {
              auto&  dst = g_board.get_square(dst_i.x,dst_i.y);

                if((&dst == &start) || dst.is_lightened())
                {
                  auto  a = ptr->get_distance();
                  auto  b =  dst.get_distance();

                    if(a > b)
                    {
                      ptr = &dst;

                      instr = e.second;
                    }
                }
            }
        }


        if(ptr == last_ptr)
        {
            if(ptr != &start)
            {
              printf("search_route error\n");
            }


          break;
        }

      else
        {
          r.codes.emplace_back(instr);
        }
    }


  return std::move(r);
}


route
search_route(point  start, point  goal) noexcept
{
  return search_route(g_board.get_square(start.x,start.y),
                      g_board.get_square( goal.x, goal.y));
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

      sq.set_total_mv_consumption(0);

      sq.unlighten();
    }}
}


basic_board<square,g_board_width,g_board_height>
g_board;


basic_board_view<square,g_board_width,g_board_height>
g_board_view;





