#include"libgbact/character.hpp"




namespace gbact{
namespace characters{


images::image
g_image;


bool
character::
m_debug;




character::
character() noexcept:
m_creation_time(g_time)
{
  initialize();
}


void
character::
initialize() noexcept
{
  static physics_parameter  p;

  static bool  initialized;

    if(!initialized)
    {
      p.set_gravitation(0.2);
    }


  m_physics.set_parameter(&p);

  show();
}


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
block(character&  target, positions::position  position) const noexcept
{
    switch(position)
    {
  case(position::left):
      target.set_right(get_area().left-1);
      break;
  case(position::right):
      target.set_left(get_area().right+1);
      break;
  case(position::top):
      target.set_bottom(get_area().top-1);
      break;
  case(position::bottom):
      target.set_top(get_area().bottom+1);
      break;
  case(position::none):
report;
      break;
    }
}


void
character::
detect_current_square() noexcept
{
  auto*  new_sq = &g_board.get_square_by_object(*this);
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

                if(!sq)
                {
                  new_sq_i.x = cur_sq_i.x;

                  set_base_point_x(sq->get_area().left-1);

                  update_area();

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

                if(!sq)
                {
                  new_sq_i.x = cur_sq_i.x;

                  set_base_point_x(sq->get_area().right+1);

                  update_area();

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

                if(!sq || sq->get_data().get_gate().test_top())
                {
                  new_sq_i.y = cur_sq_i.y;

                  set_base_point_y(sq->get_area().top-1);

                  update_area();

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

                if(!sq)
                {
                  new_sq_i.y = cur_sq_i.y;

                  set_base_point_y(sq->get_area().bottom+1);

                  update_area();

                  set_up_contacted_square(sq);

                  do_when_collided_with_square(*sq);
                }

              else
                {
                  set_up_contacted_square(nullptr);
                }
            }


          new_sq = &g_board.get_square(new_sq_i.x,new_sq_i.y);

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
check_contacted_squares() noexcept
{
  auto&  ene = get_kinetic_energy();

    if(ene.x < 0)
    {
        if(m_left_contacted_square)
        {
          ene.x = 0;
        }
    }

  else
    if(ene.x > 0)
    {
        if(m_right_contacted_square)
        {
          ene.x = 0;
        }
    }


    if(ene.y < 0)
    {
        if(m_up_contacted_square)
        {
          ene.y = 0;
        }
    }

  else
    if(ene.y > 0)
    {
        if(m_down_contacted_square)
        {
          ene.y = 0;
        }
    }
}


void
character::
update_core() noexcept
{
    if((g_time-m_creation_time) >= m_life_time)
    {
      die();

      return;
    }


    if(m_blinking_status.valid)
    {
        if(g_time >= m_blinking_status.end_time)
        {
          m_blinking_status.valid = false;
        }
    }


    if(m_physics)
    {
      auto  p = m_physics.get_parameter();

      auto&  ene = get_kinetic_energy();

      ene.y += p->get_gravitation();

      ene *= (1.0-p->get_fluid_viscosity());

      ene += p->get_fluid_kinetic_energy();
    }


  check_contacted_squares();

  object::update_core();

  detect_current_square();

  m_last_update_time = g_time;
}


void
character::
render(point  offset, image_cursor  cur) noexcept
{
    if(is_visible() && (!m_blinking_status.valid || (get_rendering_counter()&1)))
    {
      image_object::render(offset,cur);
    }


  render_additionally(offset,cur);

    if(m_debug)
    {
      auto  rect = get_rectangle();

      cur.draw_rectangle_safely(colors::red,rect.x-offset.x,rect.y-offset.y,rect.w,rect.h);


      auto&  base_pt = get_base_point();

      cur.draw_vline_safely(colors::red,base_pt.x   -offset.x,base_pt.y-32-offset.y,64);
      cur.draw_hline_safely(colors::red,base_pt.x-32-offset.x,base_pt.y   -offset.y,64);


      auto  sq = get_current_square();

        if(0 && sq)
        {
          auto&  area = sq->get_area();

          cur.fill_rectangle_safely(colors::blue,area.left-offset.x,area.top-offset.y,24,24);
        }
    }
}




}}




