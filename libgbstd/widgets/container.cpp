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


  m_first_child = nullptr;
  m_last_child  = nullptr;
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


      child->m_relative_point = point(x,y);

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
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);


  int  w = 1;
  int  h = 1;

  auto  current = m_first_child;

    while(current)
    {
      current->reform_if_needed(get_absolute_point());
 

      auto  rel_pt = current->get_relative_point();

      w = std::max(w,rel_pt.x+current->get_width() );
      h = std::max(h,rel_pt.y+current->get_height());

      current = current->m_next;
    }


  m_width  = w;
  m_height = h;
}


void
container::
redraw(image&  img) noexcept
{
  auto  current = m_first_child;

    while(current)
    {
      current->redraw_if_needed(img);

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


}}




