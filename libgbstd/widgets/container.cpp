#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
container::
clear() noexcept
{
  auto  current = m_first_child;

    while(current)
    {
      auto  next = current->m_next;

      delete current       ;
             current = next;
    }
}




void
container::
append_child(widget*  child, int  x, int  y) noexcept
{
    if(child)
    {
        if(child->m_container)
        {
          printf("append_child error: すでに親がいる\n");

          return;
        }


      child->m_container = this;

        if(!m_last_child)
        {
          m_first_child = child;
          m_last_child  = child;
        }

      else
        {
          m_last_child->m_next = child;

          child->m_previous = m_last_child        ;
                              m_last_child = child;
        }


      child->m_absolute_point = m_absolute_point+point(x,y);
      child->m_relative_point =                  point(x,y);

      child->need_to_reform();
    }
}


void
container::
remove_child(widget*  child) noexcept
{
    if(child == m_first_child)
    {
      m_first_child = child->get_next();

        if(!m_first_child)
        {
          m_last_child = nullptr;
        }
    }

  else
    if(child == m_last_child)
    {
      m_last_child = child->get_previous();

        if(!m_last_child)
        {
          m_first_child = nullptr;
        }
    }


  delete child;

  need_to_reform();
}




void
container::
render(image_cursor  cur) noexcept
{
  auto  current = m_first_child;

    while(current)
    {
        if(current->test_flag(flags::shown))
        {
          current->render(image_cursor(cur+current->get_relative_point()));
        }


      current = current->m_next;
    }
}




widget*
container::
scan_by_point(int  x, int  y) noexcept
{
    if(test_by_point(x,y))
    {
      auto  current = m_first_child;

        while(current)
        {
            if( current->test_flag(flags::shown) &&
               !current->test_flag(flags::frozen))
            {
              auto  w = current->scan_by_point(x,y);

                if(w)
                {
                  return w;
                }
            }


          current = current->m_next;
        }
    }


  return nullptr;
}


void
container::
reform() noexcept
{
  int  w = m_width ;
  int  h = m_height;

  auto  current = m_first_child;

    while(current)
    {
        if(current->test_flag(flags::needed_to_reform))
        {
          current->reform();

          w = std::max(w,current->m_absolute_end_point.x);
          h = std::max(h,current->m_absolute_end_point.y);

          current->unset_flag(flags::needed_to_reform);
        }


      current = current->m_next;
    }


  m_width  = w;
  m_height = h;

  widget::reform();
}


void
container::
redraw(image&  img) noexcept
{
  auto  current = m_first_child;

    while(current)
    {
        if(current->test_flag(flags::needed_to_redraw))
        {
          current->redraw(img);

          current->unset_flag(flags::needed_to_redraw);
        }


      current = current->m_next;
    }
}


void
container::
show_all() noexcept
{
  show();


  auto  current = m_first_child;

    while(current)
    {
      current->show_all();

      current = current->m_next;
    }
}
/*
void
widget::
redraw() noexcept
{
    if(test_flag(flags::need_to_redraw_children))
    {
      auto  current = m_first_child;

        while(current)
        {
          current->redraw();

          current = current->m_next;
        }


      unset_flag(flags::need_to_redraw_children);
    }

  else
    if(test_flag(flags::need_to_redraw_self))
    {
      render();

      unset_flag(flags::need_to_redraw_self);
    }
}
*/


}}




