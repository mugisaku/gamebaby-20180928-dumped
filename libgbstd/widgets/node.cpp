#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




background_style
node::
m_default_background_style;




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
        if(!m_current->test_by_absolute_point(mouse.point))
        {
            if(mouse.left_button)
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


    if(m_current)
    {
      m_current->update();
    }
}




void
node::
set_background_style(const background_style&  new_style) noexcept
{
  m_background_style = &new_style;

  need_to_redraw();
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
  m_background_style->render(0,0,m_width,m_height,cur);

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




