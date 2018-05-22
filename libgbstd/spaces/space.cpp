#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{


struct
space::
object_node
{
  spaces::object*  object;

  void  (*onremove)(spaces::object&  o);

  object_node*  next;

};


space::object_node*
space::
pop_node() noexcept
{
    if(m_trash)
    {
      auto  nd = m_trash                ;
                 m_trash = m_trash->next;

      return nd;
    }

  else
    {
      return new object_node;
    }
}


void
space::
push_node(object_node*  nd) noexcept
{
    if(nd)
    {
      nd->next = m_trash     ;
                 m_trash = nd;
    }
}




void
space::
append_object(object&  o, void  (*onremove)(object&  o), object_node*&  list) noexcept
{
  auto  nd = pop_node();

  nd->object = &o;
  nd->onremove = onremove;

  nd->next = list     ;
             list = nd;

  o.body::update();
}


void
space::
append_object(object&  o, void  (*onremove)(object&  o)) noexcept
{
  append_object(o,onremove,m_object_list);

  o.set_environment(nullptr);
}


void
space::
append_kinetic_object(object&  o, void  (*onremove)(object&  o)) noexcept
{
  append_object(o,onremove,m_kinetic_object_list);

  o.set_environment(&m_environment);
}




void
space::
check_collision(object&  a, object&  b) noexcept
{
    if(!spaces::area::test_collision(a.get_area(),b.get_area()))
    {
      return;
    }


  auto&   saved_area = a.get_saved_area();
  auto&  target_area = b.get_area();

  position  pos;

    if(saved_area.left >= target_area.right)
    {
      pos = position::left;
    }

  else
    if(saved_area.right <= target_area.left)
    {
      pos = position::right;
    }

  else
    if(saved_area.top >= target_area.bottom)
    {
      pos = position::top;
    }

  else
    if(saved_area.bottom <= target_area.top)
    {
      pos = position::bottom;
    }


  a.do_when_collided(b,pos);
  b.do_when_collided(a,get_opposite(pos));
}




void
space::
detect_collision() noexcept
{
    if(m_kinetic_object_list && m_kinetic_object_list->next)
    {
      auto  a = m_kinetic_object_list      ;
      auto  b = m_kinetic_object_list->next;

        while(a)
        {
          auto  b_next = b          ;
                         b = b->next;

            while(b_next)
            {
              check_collision(     *a->object,
                              *b_next->object);

              b_next = b_next->next;
            }


          a = a->next;
        }
    }


  auto  a_next = m_kinetic_object_list;

    while(a_next)
    {
      auto  b_next = m_object_list;

        while(b_next)
        {
          check_collision(*a_next->object,
                          *b_next->object);

          b_next = b_next->next;
        }


      a_next = a_next->next;
    }
}




void
space::
update_objects(object_node*  nd) noexcept
{
  object_node*  previous = nullptr;

  auto  next = nd;

    while(next)
    {
        if(next->object->is_needed_to_remove())
        {
            if(next->onremove)
            {
              next->onremove(*next->object);
            }


            if(previous)
            {
              previous->next = next->next;
            }
        }

      else
        {
          next->object->save_area();

          next->object->update();
        }


      previous = next             ;
                 next = next->next;
    }
}


void
space::
update() noexcept
{
  update_objects(        m_object_list);
  update_objects(m_kinetic_object_list);
}




void
space::
render_objects(object_node*  nd, image&  dst) const noexcept
{
  auto  next = nd;

    while(next)
    {
      next->object->render(dst);

      next = next->next;
    }
}


void
space::
render(image&  dst) const noexcept
{
  render_objects(        m_object_list,dst);
  render_objects(m_kinetic_object_list,dst);
}




}}




