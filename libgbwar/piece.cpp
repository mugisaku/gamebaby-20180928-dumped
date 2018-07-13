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

  routing_stack.emplace_back(&start);

    while(index < routing_stack.size())
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


      auto&  sq = *(routing_stack[index++]);

      auto  current_value = sq.get_total_mv_consumption();

        for(auto&  off: offset_table)
        {
          auto  dst_i = sq.get_index()+off;

            if(g_board.test_index(dst_i))
            {
              auto&  dst = g_board.get_square(dst_i.x,dst_i.y);

                if(&dst != &start)
                {
                  auto  dst_value = dst.get_total_mv_consumption();

                  auto  next_value = current_value+get_mv_consumption(dst);

                    if(!dst_value || (dst_value > next_value))
                    {
                      dst.set_total_mv_consumption(next_value);

                      dst_value = next_value;
                    }


                    if((dst_value > current_value) && (dst_value <= get_mv()))
                    {
                      auto  new_dist = sq.get_distance()+((off.x && off.y)? 133:100);

                        if(!dst.get_distance() || (new_dist < dst.get_distance()))
                        {
                          dst.set_distance(new_dist);
                        }


                      dst.lighten();

                      routing_stack.emplace_back(&dst);
                    }
                }
            }
        }
    }
}




