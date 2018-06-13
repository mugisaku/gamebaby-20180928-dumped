#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
space::
append_object(object&  o) noexcept
{
  m_object_list.emplace_back(&o);

  o.unneed_to_remove();

  o.set_space(*this);

  o.update_area();
}




void
space::
remove_all_object() noexcept
{
  m_object_list.clear();
}




void
space::
update() noexcept
{
  m_updated_object_list.clear();

    for(auto  o: m_object_list)
    {
        if(o->is_needed_to_remove())
        {
          o->unset_space();

          o->do_when_removed();
        }

      else
        {
          o->update_core();

          m_updated_object_list.emplace_back(o);
        }
    }


  m_object_list.clear();

  std::swap(m_object_list,m_updated_object_list);
}




void
space::
render(point  offset, image&  dst) const noexcept
{
    for(auto  o: m_object_list)
    {
      o->update_graphics();

      o->render(offset,dst);
    }
}




}}




