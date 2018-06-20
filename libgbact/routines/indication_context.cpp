#include"libgbact/routine.hpp"




namespace gbact{
namespace routines{




indication_context::
indication_context(rectangle  rect) noexcept:
m_point(rect),
m_rectangle(rect),
m_hand_cursor(g_misc_image,rectangle(   0,   0,24,24),point(-24,-4)),
m_rect_cursor(g_misc_image,rectangle(24*1,24*0,24,24),point())
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
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      g_object_space.append(m_hand_cursor);
      g_object_space.append(m_rect_cursor);

      set_pc(1);
      break;
  case(1):
      constexpr int  speed = 2;

           if(g_input.test_up_button()   && (m_point.y >= (m_rectangle.y+speed              ))){m_point.y -= speed;}
      else if(g_input.test_down_button() && (m_point.y <= (m_rectangle.y+m_rectangle.h-speed))){m_point.y += speed;}

           if(g_input.test_left_button()  && (m_point.x >= (m_rectangle.x                    ))){m_point.x -= speed;}
      else if(g_input.test_right_button() && (m_point.x <= (m_rectangle.x+m_rectangle.w-speed))){m_point.x += speed;}


      m_hand_cursor.set_base_point(m_point.x,m_point.y);

      m_rect_cursor.set_base_point(m_point.x/g_square_size*g_square_size,
                                   m_point.y/g_square_size*g_square_size);

        if(g_input.test_p_button() && m_callback)
        {
          m_callback(*this,m_data);
        }

      else
        if(g_input.test_n_button())
        {
          m_hand_cursor.die();
          m_rect_cursor.die();

          end(g_input);
        }
      break;
    }
}




}}




