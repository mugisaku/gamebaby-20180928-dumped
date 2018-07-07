#include"libgbstd/routine.hpp"




namespace gbstd{
namespace routines{




indication_context::
indication_context(rectangle  rect) noexcept:
m_point(rect),
m_rectangle(rect),
m_hand_cursor(g_misc_image,rectangle(0,0,24,24),point(-24,-4))
{
}




void
indication_context::
initialize(rectangle  rect) noexcept
{
  m_point     = rect;
  m_rectangle = rect;
}


void
indication_context::
set_point(int  x, int  y) noexcept
{
  m_point.x = m_rectangle.x+x;
  m_point.y = m_rectangle.y+y;

  update();
}


void
indication_context::
update() noexcept
{
    if(m_point.x < m_rectangle.x)
    {
      m_x_pooling = -(m_rectangle.x-m_point.x);

      m_point.x = m_rectangle.x;
    }

  else
    if(m_point.x >= (m_rectangle.x+m_rectangle.w))
    {
      m_x_pooling = m_point.x-(m_rectangle.x+m_rectangle.w);

      m_point.x = m_rectangle.x+m_rectangle.w;
    }

  else
    {
      m_x_pooling = 0;
    }


    if(m_point.y < m_rectangle.y)
    {
      m_y_pooling = -(m_rectangle.y-m_point.y);

      m_point.y = m_rectangle.y;
    }

  else
    if(m_point.y >= (m_rectangle.y+m_rectangle.h))
    {
      m_y_pooling = m_point.y-(m_rectangle.y+m_rectangle.h);

      m_point.y = m_rectangle.y+m_rectangle.h;
    }

  else
    {
      m_y_pooling = 0;
    }


  m_hand_cursor.set_base_point(m_point.x,m_point.y);
}




void
indication_context::
clean() noexcept
{
  m_hand_cursor.die();
}


void
indication_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      g_screen_object_space.append(m_hand_cursor);

      set_pc(1);
      break;
  case(1):
        if(g_time >= m_next_time)
        {
          m_next_time = g_time+m_wait_time;

               if(g_input.test_up_button()  ){m_point.y -= m_speed;}
          else if(g_input.test_down_button()){m_point.y += m_speed;}

               if(g_input.test_left_button() ){m_point.x -= m_speed;}
          else if(g_input.test_right_button()){m_point.x += m_speed;}


          update();

          execute_callback();
        }
      break;
    }
}




}}




