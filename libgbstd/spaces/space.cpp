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
append_object(object&  o, bool  use_env) noexcept
{
  auto  nd = pop_node_from_trash();

  nd->object = &o;

  nd->next = m_object_list     ;
             m_object_list = nd;

  o.unneed_to_remove();

  o.set_space(*this);
  o.set_environment(use_env? &m_environment:nullptr);

  o.body::update();
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
    if(!boards::area::test_collision(a.get_area(),b.get_area()))
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
    if(m_object_list && m_object_list->next)
    {
      auto  a_current = m_object_list      ;
      auto  b         = m_object_list->next;

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
}


void
space::
detect_collision(boards::board&  board) noexcept
{
  auto  current = m_object_list;

    while(current)
    {
      board.detect_collision(*current->object);

      current = current->next;
    }
}




void
space::
update() noexcept
{
  object_node*  previous = nullptr;

  auto  current = m_object_list;

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

            if(current == m_object_list)
            {
              m_object_list = next;
            }


          obj.do_when_removed();
        }

      else
        {
          obj.save_area();

          obj.update_core();

          previous = current;
        }


      current = next;
    }
}




void
space::
render(point  offset, image&  dst) const noexcept
{
  auto  current = m_object_list;

    while(current)
    {
      current->object->update_graphics();

      current->object->render(offset,dst);

      current = current->next;
    }
}




}}




