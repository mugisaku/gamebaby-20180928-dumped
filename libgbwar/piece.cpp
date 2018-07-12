#include"libgbwar/piece.hpp"
#include"libgbwar/board.hpp"




image
piece::
g_image;


void
piece::
set_square(square*  sq) noexcept
{
  m_square = sq;

    if(sq)
    {
      m_object.set_base_point(sq->get_index()*square::size);
    }
}


void
piece::
seek_route() const noexcept
{
    if(!m_square)
    {
      return;
    }


  int  index = 0;

  auto&  start = *m_square;


  static std::vector<square*>  routing_stack;

  routing_stack.clear();

  start.set_mv(get_mv());

  routing_stack.emplace_back(&start);

    while(index < routing_stack.size())
    {
      auto&  sq = *(routing_stack[index++]);

      auto  mv = sq.get_mv();

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
          auto  dst_i = sq.get_index()+off;

            if(g_board.test_index(dst_i))
            {
              auto&  dst = g_board.get_square(dst_i.x,dst_i.y);

                if(dst.get_mv_consumption() < 0)
                {
                  dst.set_mv_consumption(get_mv_consumption(dst));
                }


              auto  mv_consumption = dst.get_mv_consumption();

                if((mv_consumption <= mv) && (dst.get_mv() < mv))
                {
                  dst.set_mv(mv-mv_consumption);

                  routing_stack.emplace_back(&dst);
                }
            }
        }
    }
}




