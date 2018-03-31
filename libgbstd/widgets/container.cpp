#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
container::
clear() noexcept
{
  m_children.clear();
}




void
container::
append_child(widget*  child, int  x, int  y) noexcept
{
    if(child)
    {
        if(child->get_parent())
        {
          printf("append_child error: すでに親がいる\n");

          return;
        }


      child->set_parent(this);

      m_children.emplace_back(child);

      child->set_relative_point(point(x,y));

      child->need_to_reform();
    }
}


bool
container::
remove(widget*  target) noexcept
{
    for(auto&  child: m_children)
    {
        if(child->remove(target))
        {
          need_to_reform();

          return true;
        }
    }


  return false;
}




widget*
container::
scan_by_point(int  x, int  y) noexcept
{
    if(test_by_point(x,y))
    {
        for(auto&  child: m_children)
        {
            if( child->test_flag(flags::shown) &&
               !child->test_flag(flags::frozen))
            {
              auto  w = child->scan_by_point(x,y);

                if(w)
                {
                  return w;
                }
            }
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

    for(auto&  child: m_children)
    {
      child->reform_if_needed(get_absolute_point());
 

      auto  rel_pt = child->get_relative_point();

      w = std::max(w,rel_pt.x+child->get_width() );
      h = std::max(h,rel_pt.y+child->get_height());
    }


  m_width  = w;
  m_height = h;
}


void
container::
redraw(image&  img) noexcept
{
    for(auto&  child: m_children)
    {
        if(child->test_flag(flags::shown))
        {
          child->redraw_if_needed(img);
        }
    }


  unset_flag(flags::needed_to_redraw);
}


void
container::
show_all() noexcept
{
  show();

    for(auto&  child: m_children)
    {
      child->show_all();
    }
}




void
container::
print(int  indent) const noexcept
{
  widget::print(indent);

  printf_with_indent(indent+2,"{\n");

  auto   it = m_children.cbegin();
  auto  end = m_children.cend();

    if(it != end)
    {
      (*it)->print(indent+2);

        while(++it != end)
        {
          printf_with_indent(indent+2,",\n");

          (*it)->print(indent+2);
        }
    }


  printf_with_indent(indent,"\n}");
}




}}




