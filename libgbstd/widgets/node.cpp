#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
node::
set_root(root*  r) noexcept
{
  widget::set_root(r);

    if(m_target)
    {
      m_target->set_root(r);
    }
}


void
node::
cancel_current() noexcept
{
    if(m_current)
    {
      m_current->do_when_cursor_got_out();

      m_current = nullptr;
    }
}




void
node::
do_when_cursor_got_in()  noexcept
{
}


void
node::
check_by_mouse(control_devices::mouse&  mouse) noexcept
{
    if(!m_current->test_by_absolute_point(mouse.point))
    {
        if(mouse.left_button ||
           mouse.right_button)
        {
          auto  abs_pt = m_current->get_absolute_point();

          int    left = abs_pt.x                        ;
          int   right = abs_pt.x+m_current->get_width() ;
          int     top = abs_pt.y                        ;
          int  bottom = abs_pt.y+m_current->get_height();

               if(mouse.point.x <   left){mouse.point.x =  left  ;}
          else if(mouse.point.x >= right){mouse.point.x = right-1;}

               if(mouse.point.y <     top){mouse.point.y =    top  ;}
          else if(mouse.point.y >= bottom){mouse.point.y = bottom-1;}
        }

      else
        {
          m_current->do_when_cursor_got_out();

          m_current = m_target->scan_by_absolute_point(mouse.point);

            if(m_current)
            {
              m_current->do_when_cursor_got_in();
            }
        }
    }
}


void
node::
do_when_cursor_got_out() noexcept
{
    if(m_current)
    {
      auto  root = get_root();

      check_by_mouse(root->get_mouse());
    }
}


void
node::
update() noexcept
{
    if(!m_target)
    {
      return;
    }


  auto  root = get_root();

  auto&  mouse = root->get_mouse();

    if(!m_current)
    {
      m_current = m_target->scan_by_absolute_point(mouse.point);

        if(m_current)
        {
          m_current->do_when_cursor_got_in();
        }
    }

  else
    if(root->is_mouse_moved())
    {
      check_by_mouse(mouse);
    }


    if(m_current)
    {
      m_current->update();
    }
}




void
node::
set_target(widget*  target) noexcept
{
    if(target)
    {
        if(target->get_parent())
        {
          printf("wrapper set_target error: すでに親がいる\n");

          return;
        }


      target->set_parent(this);
    }


  m_target.reset(target);

  m_current = nullptr;

  need_to_reform();
}


void
node::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

    if(m_target)
    {
      m_target->reform_if_needed(get_absolute_point());
 

      auto  rel_pt = m_target->get_relative_point();

      m_width  = rel_pt.x+m_target->get_width() ;
      m_height = rel_pt.y+m_target->get_height();
    }

  else
    {
      m_width  = 1;
      m_height = 1;
    }
}


void
node::
render(image_cursor  cur) noexcept
{
  render_background(cur);

    if(m_target)
    {
      m_target->redraw(cur.get_image());
    }
}


void
node::
show_all() noexcept
{
  show();

  m_target->show_all();
}




void
node::
print(int  indent) const noexcept
{
  m_target->print(indent);
}




}}




