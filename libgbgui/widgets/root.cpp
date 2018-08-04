#include"libgbgui/widget.hpp"



namespace gbgui{
namespace widgets{




void
root::
set_node_target(widget*  w) noexcept
{
  m_node.set_target(w);

  m_node.set_root(this);

  m_node.show_all();

  m_node.reform(point());
}


void
root::
push_widget_that_needed_to_redraw(widget&  w) noexcept
{
    if(m_redrawing_last)
    {
      m_redrawing_last->set_next(&w);
    }

  else
    {
      m_redrawing_first = &w;
    }


  m_redrawing_last = &w;

  w.set_next(nullptr);
}




void
root::
react(const control_device&  condev) noexcept
{
  m_node.reform_if_needed(point());

  auto  m = condev.mouse;

    for(auto&  pt: condev.point_list)
    {
      m.point = pt;

      m_old_mouse = m_mouse    ;
                    m_mouse = m;

      m_mouse.point -= m_offset;


      m_node.update();
    }


  m_node.reform_if_needed(point());
}


void
root::
redraw(image&  img) noexcept
{
  m_node.reform_if_needed(point());

  m_node.redraw(img);
}


bool
root::
redraw_only_needed_widgets(image&  img) noexcept
{
  m_node.reform_if_needed(point());

    if(m_redrawing_first)
    {
        while(m_redrawing_first)
        {
          m_redrawing_first->redraw(img);

          m_redrawing_first = m_redrawing_first->get_next();
        }


      m_redrawing_first = nullptr;
      m_redrawing_last  = nullptr;

      return true;
    }


  return false;
}




}}




