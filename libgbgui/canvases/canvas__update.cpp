#include"libgbgui/canvas.hpp"



namespace gbgui{
namespace canvases{




void
canvas::
update() noexcept
{
  auto  mouse = get_mouse();

  int  x = mouse.point.x/m_pixel_size;
  int  y = mouse.point.y/m_pixel_size;

  bool  cursor_moved = (m_current_point != point(x,y));

  m_current_point.x = x;
  m_current_point.y = y;

    if(cursor_moved && m_callback)
    {
      m_callback(*this,canvas_event::painting_cursor_moved);
    }


    switch(m_mode)
    {
  case(mode::draw_dot):
        if(mouse.left_button)
        {
          modify_dot(m_drawing_color,x,y);

            if(m_callback)
            {
              m_callback(*this,canvas_event::image_is_modified);
            }
        }

      else
        if(mouse.right_button)
        {
          modify_dot(color(),x,y);

            if(m_callback)
            {
              m_callback(*this,canvas_event::image_is_modified);
            }
        }
      break;
  case(mode::draw_line):
        if(m_pointing_count)
        {
            if(mouse.left_button)
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_image_cursor.get_image());

                  need_to_redraw();
                }


              draw_line(m_drawing_color,m_a_point,point(x,y));
            }

          else
            if(mouse.right_button)
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_image_cursor.get_image());

                  need_to_redraw();
                }


              draw_line(color(),m_a_point,point(x,y));
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(mouse.left_button ||
               mouse.right_button)
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::draw_rectangle):
        if(m_pointing_count)
        {
            if(mouse.left_button)
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_image_cursor.get_image());

                  need_to_redraw();
                }


              draw_rect(m_drawing_color,make_rectangle(m_a_point,point(x,y)));
            }

          else
            if(mouse.right_button)
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_image_cursor.get_image());

                  need_to_redraw();
                }


              draw_rect(color(),make_rectangle(m_a_point,point(x,y)));
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(mouse.left_button ||
               mouse.right_button)
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_rectangle):
        if(m_pointing_count)
        {
            if(mouse.left_button)
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_image_cursor.get_image());

                  need_to_redraw();
                }


              fill_rect(m_drawing_color,make_rectangle(m_a_point,point(x,y)));
            }

          else
            if(mouse.right_button)
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_image_cursor.get_image());

                  need_to_redraw();
                }


              fill_rect(color(),make_rectangle(m_a_point,point(x,y)));
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(mouse.left_button ||
               mouse.right_button)
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_area):
        if(mouse.left_button)
        {
          fill_area(m_drawing_color,point(x,y));
        }

      else
        if(mouse.right_button)
        {
          fill_area(color(),point(x,y));
        }
      break;
  case(mode::select):
        if(m_pointing_count)
        {
            if(mouse.left_button)
            {
              m_operation_rect = make_rectangle(m_a_point,point(x,y));

              need_to_redraw();
            }

          else
            {
              m_pointing_count = 0;

              need_to_redraw();
            }
        }

      else
        {
            if(mouse.left_button)
            {
              m_a_point = point(x,y);

              m_pointing_count = 1;
            }

          else
            if(mouse.right_button)
            {
              m_operation_rect = rectangle(0,0,m_editing_width,m_editing_height);

              need_to_redraw();
            }
        }
      break;
  case(mode::paste):
      m_a_point = point(x,y);

        if(mouse.left_button)
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_a_point != m_b_point)
        {
            if(!m_drawing_is_fixed)
            {
              m_recorder.rollback(m_image_cursor.get_image());

              need_to_redraw();
            }


          m_b_point = m_a_point;

          paste(m_a_point,false);
        }
      break;
  case(mode::layer):
      m_a_point = point(x,y);

        if(mouse.left_button)
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_a_point != m_b_point)
        {
            if(!m_drawing_is_fixed)
            {
              m_recorder.rollback(m_image_cursor.get_image());

              need_to_redraw();
            }


          m_b_point = m_a_point;

          paste(m_a_point,true);
        }
      break;
    }
}




}}




