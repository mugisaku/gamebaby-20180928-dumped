#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




void
container::
set_root(root*  r) noexcept
{
    if(get_root() != r)
    {
      widget::set_root(r);

        for(auto&  child: m_children)
        {
          child->set_root(r);
        }
    }
}


void
container::
change_current_by_name(gbstd::string_view  name) noexcept
{
    for(auto&  child: m_children)
    {
        if(child->get_name() == name)
        {
          m_current = child.get();

          need_to_reform();

          return;
        }
    }


  printf("container change_current_by_name error: %sというウィジェットは無い",name.data());
}


void
container::
append_child(widget*  child, int  x, int  y) noexcept
{
    if(child)
    {
        if(child->get_parent())
        {
          printf("container append_child error: すでに親がいる\n");

          return;
        }


      child->set_parent(this);

      child->set_root(get_root());


      m_children.emplace_back(child);

      child->set_relative_point(point(x,y));

      child->need_to_reform();

        if(!m_current)
        {
          m_current = child;
        }
    }
}




widget*
container::
scan_by_absolute_point(int  x, int  y) noexcept
{
    if(test_by_absolute_point(x,y))
    {
        if(m_unique)
        {
            if(m_current &&
               m_current->test_flag(flags::shown))
            {
              auto  w = m_current->scan_by_absolute_point(x,y);

                if(w)
                {
                  return w;
                }
            }
        }

      else
        {
            for(auto&  child: m_children)
            {
                if(child->test_flag(flags::shown))
                {
                  auto  w = child->scan_by_absolute_point(x,y);

                    if(w)
                    {
                      return w;
                    }
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

    if(m_unique)
    {
        if(m_current)
        {
          m_current->reform_if_needed(get_absolute_point());
     

          auto  rel_pt = m_current->get_relative_point();

          w = std::max(w,rel_pt.x+m_current->get_width() );
          h = std::max(h,rel_pt.y+m_current->get_height());
        }
    }

  else
    {
        for(auto&  child: m_children)
        {
          child->reform_if_needed(get_absolute_point());
     

          auto  rel_pt = child->get_relative_point();

          w = std::max(w,rel_pt.x+child->get_width() );
          h = std::max(h,rel_pt.y+child->get_height());
        }
    }


  m_width  = w;
  m_height = h;
}


void
container::
render(image_cursor  cur) noexcept
{
  auto&  img = cur.get_image();

    if(m_unique)
    {
        if(m_current && m_current->test_flag(flags::shown))
        {
          m_current->redraw(img);
        }
    }

  else
    {
        for(auto&  child: m_children)
        {
            if(child->test_flag(flags::shown))
            {
              child->redraw(img);
            }
        }
    }
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




