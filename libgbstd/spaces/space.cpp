#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{


struct
space::
object_node
{
  spaces::object*  object;

  object_node*  next;

};


space::object_node*
space::
pop_node_from_trash() noexcept
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
push_node_to_trash(object_node*  nd) noexcept
{
    if(nd)
    {
      nd->next = m_trash     ;
                 m_trash = nd;
    }
}




void
space::
append_object(object&  o, object_node*&  list) noexcept
{
  auto  nd = pop_node_from_trash();

  nd->object = &o;

  nd->next = list     ;
             list = nd;

  o.unneed_to_remove();
  o.set_space(*this);

  o.body::update();
}


void
space::
append_object(object&  o) noexcept
{
  append_object(o,m_object_list);

  o.set_environment(nullptr);
}


void
space::
append_kinetic_object(object&  o) noexcept
{
  append_object(o,m_kinetic_object_list);

  o.set_environment(&m_environment);
}




void
space::
remove_all_object() noexcept
{
  auto  current = m_object_list          ;
                  m_object_list = nullptr;

    while(current)
    {
      auto  next = current->next;

      push_node_to_trash(current)      ;
                         current = next;
    }


  current = m_kinetic_object_list          ;
            m_kinetic_object_list = nullptr;

    while(current)
    {
      auto  next = current->next;

      push_node_to_trash(current)      ;
                         current = next;
    }
}




void
space::
empty_trash() noexcept
{
    while(m_trash)
    {
      auto  next = m_trash->next;

      delete m_trash       ;
             m_trash = next;
    }


  m_trash = nullptr;
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
      auto  a_current = m_kinetic_object_list      ;
      auto  b         = m_kinetic_object_list->next;

        while(b)
        {
          auto  b_current = b          ;
                            b = b->next;

            while(b_current)
            {
              check_collision(*a_current->object,
                              *b_current->object);

              b_current = b_current->next;
            }


          a_current = a_current->next;
        }
    }


  auto  a = m_kinetic_object_list;

    while(a)
    {
      auto  b = m_object_list;

        while(b)
        {
          check_collision(*a->object,
                          *b->object);

          b = b->next;
        }


      a = a->next;
    }
}




void
space::
update_objects(object_node*&  list) noexcept
{
  object_node*  previous = nullptr;

  auto  current = list;

    while(current)
    {
      auto  next = current->next;

      auto&  obj = *current->object;

        if(obj.is_needed_to_remove())
        {
          obj.unset_space();

            if(previous)
            {
              previous->next = next;
            }


          push_node_to_trash(current);

            if(current == list)
            {
              list = nullptr;
            }


          obj.do_when_removed();
        }

      else
        {
          obj.save_area();

          obj.update();

          previous = current;
        }


      current = next;
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
render_objects(object_node*  list, image&  dst) const noexcept
{
  auto  current = list;

    while(current)
    {
      current->object->render(dst);

      current = current->next;
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




