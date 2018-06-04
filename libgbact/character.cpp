#include"libgbact/character.hpp"
#include"libgbact/block.hpp"




namespace gbact{
namespace characters{


images::image
g_image;




bool
character::
check_last_animated_time(uint32_t  interval) noexcept
{
    if(g_time >= (m_last_animated_time+interval))
    {
      m_last_animated_time = g_time;

      ++m_phase;

      return true;
    }


  return false;
}


void
character::
blink(uint32_t  time) noexcept
{
  m_blinking_status.valid = true;

  m_blinking_status.end_time = g_time+time;
}


void
character::
do_when_collided(object&  other_side, positions::position  position) noexcept
{
    if(other_side.get_kind_code() == kind_codes::player)
    {
      do_when_collided_with_player(static_cast<player&>(other_side),position);
    }

  else
    if(other_side.get_kind_code() == kind_codes::bullet)
    {
      do_when_collided_with_bullet(static_cast<bullet&>(other_side),position);
    }

  else
    {
      do_when_collided_with_object(other_side,position);
    }
}


bool
character::
test_if_can_move_into_square(boards::square&  sq) const noexcept
{
  auto  blk = static_cast<blocks::block*>(sq.get_data());

  return !blk || !blk->test_wall_flag();
}


void
character::
step(boards::board&  board) noexcept
{
  auto&  area = get_area();
  auto&    pt = get_base_point();

  auto  sq_size = board.get_square_size();

  auto*  new_sq = &board.get_square_by_object(*this);
  auto*  cur_sq = get_current_square();

    if(new_sq != cur_sq)
    {
        if(cur_sq)
        {
          auto  cur_sq_i = cur_sq->get_index();
          auto  new_sq_i = new_sq->get_index();

            if(cur_sq_i.x < new_sq_i.x)
            {
              set_left_contacted_square(nullptr);
              set_down_contacted_square(nullptr);

              auto  sq = cur_sq->get_link(boards::links::right);

                if(!sq || !test_if_can_move_into_square(*sq))
                {
                  new_sq_i.x = cur_sq_i.x;

                  set_base_point_x(sq->get_area().left-1);

                  body::update();

                  set_right_contacted_square(sq);

                  do_when_collided_with_square(*sq);
                }

              else
                {
                  set_right_contacted_square(nullptr);
                }
            }

          else
            if(cur_sq_i.x > new_sq_i.x)
            {
              set_right_contacted_square(nullptr);
              set_down_contacted_square(nullptr);

              auto  sq = cur_sq->get_link(boards::links::left);

                if(!sq || !test_if_can_move_into_square(*sq))
                {
                  new_sq_i.x = cur_sq_i.x;

                  set_base_point_x(sq->get_area().right+1);

                  body::update();

                  set_left_contacted_square(sq);

                  do_when_collided_with_square(*sq);
                }

              else
                {
                  set_left_contacted_square(nullptr);
                }
            }


            if(cur_sq_i.y < new_sq_i.y)
            {
              set_up_contacted_square(nullptr);

              auto  sq = cur_sq->get_link(boards::links::down);

                if(!sq || !test_if_can_move_into_square(*sq))
                {
                  new_sq_i.y = cur_sq_i.y;

                  set_base_point_y(sq->get_area().top-1);

                  body::update();

                  set_down_contacted_square(sq);

                  do_when_collided_with_square(*sq);
                }

              else
                {
                  set_down_contacted_square(nullptr);
                }
            }

          else
            if(cur_sq_i.y > new_sq_i.y)
            {
              set_down_contacted_square(nullptr);

              auto  sq = cur_sq->get_link(boards::links::up);

                if(!sq || !test_if_can_move_into_square(*sq))
                {
                  new_sq_i.y = cur_sq_i.y;

                  set_base_point_y(sq->get_area().bottom+1);

                  body::update();

                  set_up_contacted_square(sq);

                  do_when_collided_with_square(*sq);
                }

              else
                {
                  set_up_contacted_square(nullptr);
                }
            }


          new_sq = &board.get_square(new_sq_i.x,new_sq_i.y);

            if(new_sq != cur_sq)
            {
              set_current_square(new_sq);

              do_when_changed_square(new_sq,cur_sq);
            }
        }

      else
        {
          set_current_square(new_sq);

          do_when_changed_square(new_sq,cur_sq);
        }
    }
}


void
character::
update_core() noexcept
{
    if(m_blinking_status.valid)
    {
        if(g_time >= m_blinking_status.end_time)
        {
          m_blinking_status.valid = false;
        }
    }


  object::update_core();

  m_last_update_time = g_time;
}


void
character::
render(point  offset, images::image&  dst) noexcept
{
  ++m_rendering_count;

    if(m_visible && (!m_blinking_status.valid || (m_rendering_count&1)))
    {
      image_object::render(offset,dst);
    }


    if(1)
    {
      auto  rect = get_rectangle();

      dst.draw_rectangle_safely(colors::red,rect.x-offset.x,rect.y-offset.y,rect.w,rect.h);


      auto&  base_pt = get_base_point();

      dst.draw_vline_safely(colors::red,base_pt.x   -offset.x,base_pt.y-32-offset.y,64);
      dst.draw_hline_safely(colors::red,base_pt.x-32-offset.x,base_pt.y   -offset.y,64);


      auto  sq = get_current_square();

        if(0 && sq)
        {
          auto&  area = sq->get_area();

          dst.fill_rectangle_safely(colors::blue,area.left-offset.x,area.top-offset.y,24,24);
        }
    }
}




}}




