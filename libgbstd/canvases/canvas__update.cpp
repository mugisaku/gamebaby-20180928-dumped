#include"libgbstd/canvas.hpp"



namespace gbstd{
namespace canvases{




void
canvas::
update() noexcept
{
  auto  mouse = get_mouse();

  int  x = mouse.point.x/m_pixel_size;
  int  y = mouse.point.y/m_pixel_size;

    switch(m_mode)
    {
  case(mode::draw_dot):
        if(mouse.left_button)
        {
          modify_dot(m_drawing_color,x,y);

            if(m_callback)
            {
              m_callback(*this);
            }
        }

      else
        if(mouse.right_button)
        {
          modify_dot(color(),x,y);

            if(m_callback)
            {
              m_callback(*this);
            }
        }
      break;
  case(mode::draw_line):
        if(m_pointing_count)
        {
            if(mouse.left_button)
            {
                if(m_recorder.get_count())
                {
                  m_recorder.rollback(*m_image);
                }


              draw_line(m_drawing_color,m_a_point,point(x,y));
            }

          else
            if(mouse.right_button)
            {
                if(m_recorder.get_count())
                {
                  m_recorder.rollback(*m_image);
                }


              draw_line(color(),m_a_point,point(x,y));
            }

          else
            {
              m_recorder.reset_count();

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
                if(m_recorder.get_count())
                {
                  m_recorder.rollback(*m_image);
                }


              draw_rect(m_drawing_color,make_rectangle(m_a_point,point(x,y)));
            }

          else
            if(mouse.right_button)
            {
                if(m_recorder.get_count())
                {
                  m_recorder.rollback(*m_image);
                }


              draw_rect(color(),make_rectangle(m_a_point,point(x,y)));
            }

          else
            {
              m_recorder.reset_count();

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
                if(m_recorder.get_count())
                {
                  m_recorder.rollback(*m_image);
                }


              fill_rect(m_drawing_color,make_rectangle(m_a_point,point(x,y)));
            }

          else
            if(mouse.right_button)
            {
                if(m_recorder.get_count())
                {
                  m_recorder.rollback(*m_image);
                }


              fill_rect(color(),make_rectangle(m_a_point,point(x,y)));
            }

          else
            {
              m_recorder.reset_count();

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
    }
}




}}




