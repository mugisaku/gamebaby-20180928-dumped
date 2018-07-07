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

  m_square_point_offset.y = -(square::size/2);

  m_life_area.top    = 0;
  m_life_area.left   = 0;
  m_life_area.right  = g_board.get_image_width();
  m_life_area.bottom = g_board.get_image_height();

  m_creation_time = g_time;

  m_left_contacted_square   = nullptr;
  m_right_contacted_square  = nullptr;
  m_top_contacted_square    = nullptr;
  m_bottom_contacted_square = nullptr;

  m_current_square = nullptr;

  m_blinking_status.valid    = false;
  m_blinking_status.visible  = false;
  m_blinking_status.end_time = 0;

  m_holding = false;

  set_kinetic_energy(0,0);
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
      break;
    }
}


namespace{
boards::square*
get_square(int  x, int  y) noexcept
{
    if((x >= 0) &&
       (y >= 0) &&
       (x <  g_board.get_width()) &&
       (y <  g_board.get_height()))
    {
      return &g_board.get_square(x,y);
    }


  return nullptr;
}
}


void
character::
detect_current_square() noexcept
{
  auto    left = get_area().left;
  auto   right = get_area().right;
  auto     top = get_area().top;
  auto  bottom = get_area().bottom;

  auto&  base_pt = get_base_point();

  int  base_y = (static_cast<int>(base_pt.y)+m_square_point_offset.y)/square::size;

    if(!is_holding())
    {
        if(did_move_to_left())
        {
          auto  x = left/square::size;

          auto  sq = get_square(x,base_y);

            if(sq && sq->get_data().get_gate().test_right())
            {
              set_left(sq->get_area().right);

              m_left_contacted_square = sq;

              set_kinetic_energy_x(0);

              do_when_collided_with_square(*sq);
            }

          else
            {
              m_left_contacted_square = nullptr;
            }
        }

      else
        if(did_move_to_right())
        {
          auto  x = right/square::size;

          auto  sq = get_square(x,base_y);

            if(sq && sq->get_data().get_gate().test_left())
            {
              set_right(sq->get_area().left);

              m_right_contacted_square = sq;

              set_kinetic_energy_x(0);

              do_when_collided_with_square(*sq);
            }

          else
            {
              m_right_contacted_square = nullptr;
            }
        }
    }


  int  base_x = (static_cast<int>(base_pt.x)+m_square_point_offset.x)/square::size;

    if(!is_holding())
    {
        if(did_move_to_up())
        {
          auto  y = top/square::size;

          auto  sq = get_square(base_x,y);

            if(sq && sq->get_data().get_gate().test_bottom())
            {
              set_top(sq->get_area().bottom);

              m_top_contacted_square = sq;

              set_kinetic_energy_y(0);

              do_when_collided_with_square(*sq);
            }

          else
            {
              m_top_contacted_square = nullptr;
            }
        }

      else
        if(did_move_to_down())
        {
          auto  y = bottom/square::size;

          auto  sq = get_square(base_x,y);

            if(sq && sq->get_data().get_gate().test_top())
            {
              set_bottom(sq->get_area().top);

              m_bottom_contacted_square = sq;

              set_kinetic_energy_y(0);

              do_when_collided_with_square(*sq);
            }

          else
            {
              m_bottom_contacted_square = nullptr;
            }
        }
    }


  base_y = (static_cast<int>(base_pt.y)+m_square_point_offset.y)/square::size;

  auto  last_square = m_current_square                            ;
                      m_current_square = get_square(base_x,base_y);

  do_when_changed_square(m_current_square,last_square);
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


    if((get_area().left   > m_life_area.right ) ||
       (get_area().top    > m_life_area.bottom) ||
       (get_area().right  < m_life_area.left  ) ||
       (get_area().bottom < m_life_area.top   ))
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


  detect_current_square();

    if(is_holding())
    {
      set_kinetic_energy(0,0);
    }


    if(is_landing())
    {
      set_kinetic_energy_x(0);
    }


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


        if(m_current_square)
        {
          auto&  area = m_current_square->get_area();

          cur.fill_rectangle_safely(colors::blue,area.left-offset.x,area.top-offset.y,24,24);
        }
    }
}




}}




