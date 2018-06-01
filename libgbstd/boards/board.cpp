#include"libgbstd/board.hpp"
#include"libgbstd/space.hpp"




namespace gbstd{
namespace boards{




int
board::
get_corrected_x(int  x) const noexcept
{
  x %= m_image_width;

    if(x < 0)
    {
      x += m_image_width;
    }


  return x;
}


int
board::
get_corrected_y(int  y) const noexcept
{
  y %= m_image_height;

    if(y < 0)
    {
      y += m_image_height;
    }


  return y;
}




void
board::
build(int  w, int  h, int  square_size) noexcept
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

      boards::area  area;

      area.top    = y*square_size;
      area.left   = x*square_size;
      area.right  = area.left+square_size-1;
      area.bottom = area.top +square_size-1;

      sq.set_area(area);

      sq.set_index(point(x,y));


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


void
board::
put_to_around(square_data*  sqdat) noexcept
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


namespace{
void
do_when_entered(spaces::object&  o, square*  sq) noexcept
{
    if(sq)
    {
      o.do_when_entered(*sq);
    }
}
}


void
board::
detect_collision(spaces::object&  o) noexcept
{
  auto&  area = o.get_area();
  auto&    pt = o.get_base_point();

  auto  base_x = get_corrected_x(pt.x)/m_square_size;
  auto  base_y = get_corrected_y(pt.y)/m_square_size;

  auto*  new_sq = &get_square(base_x,base_y);
  auto*  cur_sq = o.get_current_square();

    if(new_sq != cur_sq)
    {
        if(cur_sq)
        {
          auto  cur_sq_i = cur_sq->get_index();
          auto  new_sq_i = new_sq->get_index();

            if(cur_sq_i.x < new_sq_i.x)
            {
              o.set_left_contacted_square(nullptr);
              o.set_down_contacted_square(nullptr);

              auto     sq = cur_sq->get_link(links::right);
              auto  up_sq =     sq->get_link(links::up);

                if(!sq || sq->get_data() || !up_sq || up_sq->get_data())
                {
                  new_sq_i.x = cur_sq_i.x;

                  o.set_base_point_x(sq->get_area().left-1);

                  o.body::update();

                  o.set_right_contacted_square(sq);
                }

              else
                {
                  o.set_right_contacted_square(nullptr);
                }
            }

          else
            if(cur_sq_i.x > new_sq_i.x)
            {
              o.set_right_contacted_square(nullptr);
              o.set_down_contacted_square(nullptr);

              auto     sq = cur_sq->get_link(links::left);
              auto  up_sq =     sq->get_link(links::up);

                if(!sq || sq->get_data() || !up_sq || up_sq->get_data())
                {
                  new_sq_i.x = cur_sq_i.x;

                  o.set_base_point_x(sq->get_area().right+1);

                  o.body::update();

                  o.set_left_contacted_square(sq);
                }

              else
                {
                  o.set_left_contacted_square(nullptr);
                }
            }


            if(cur_sq_i.y < new_sq_i.y)
            {
              o.set_up_contacted_square(nullptr);

              auto  sq = cur_sq->get_link(links::down);

                if(!sq || sq->get_data())
                {
                  new_sq_i.y = cur_sq_i.y;

                  o.set_base_point_y(sq->get_area().top-1);

                  o.body::update();

                  o.set_down_contacted_square(sq);
                }

              else
                {
                  o.set_down_contacted_square(nullptr);
                }
            }

          else
            if(cur_sq_i.y > new_sq_i.y)
            {
              o.set_down_contacted_square(nullptr);

              auto     sq = cur_sq->get_link(links::up);
              auto  up_sq =     sq->get_link(links::up);

                if(!sq || sq->get_data() || !up_sq || up_sq->get_data())
                {
                  new_sq_i.y = cur_sq_i.y;

                  o.set_base_point_y(sq->get_area().bottom+1);

                  o.body::update();

                  o.set_up_contacted_square(sq);
                }

              else
                {
                  o.set_up_contacted_square(nullptr);
                }
            }


          new_sq = &get_square(new_sq_i.x,new_sq_i.y);
        }


      o.set_current_square(new_sq);
    }
}




}}




