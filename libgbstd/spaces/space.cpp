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
append_object(object&  o, object_node*&  first, object_node*&  last) noexcept
{
  auto  nd = pop_node();

  nd->object = &o;

  o.set_space(*this);

    if(last)
    {
      last->next = nd;
    }

  else
    {
      first = nd;
    }


  last = nd;

  o.body::update();

  nd->next = nullptr;
}


void
space::
append_dynamical_object(object&  o) noexcept
{
  append_object(o,m_dynamicals_first,m_dynamicals_last);
}


void
space::
append_statical_object(object&  o) noexcept
{
  append_object(o,m_staticals_first,m_staticals_last);
}




void
space::
append_child(space&  sp, int  x, int  y) noexcept
{
  m_child_spaces.emplace_back(&sp);

  sp.m_parent_space = this;

  sp.m_relative_point = point(x,y);
}


void
space::
reform() noexcept
{
  m_absolute_point = m_relative_point;

    if(m_parent_space)
    {
      m_absolute_point += m_parent_space->m_absolute_point;
    }


    for(auto  sp: m_child_spaces)
    {
      sp->reform();
    }


  m_right_position  = m_absolute_point.x+m_width ;
  m_bottom_position = m_absolute_point.y+m_height;
}




void
space::
detect_dd_collision(void  (*callback)(object&  a, object&  b)) noexcept
{
    if(!m_dynamicals_first || !m_dynamicals_first->next)
    {
      return;
    }

  auto  a = m_dynamicals_first      ;
  auto  b = m_dynamicals_first->next;

    while(a)
    {
      auto  b_next = b          ;
                     b = b->next;

        while(b_next)
        {
          callback(*a->object,*b_next->object);

          b_next = b_next->next;
        }


      a = a->next;
    }
}


void
space::
detect_ds_collision(void  (*callback)(object&  a, object&  b)) noexcept
{
  auto  a_next = m_dynamicals_first;

    while(a_next)
    {
      auto  b_next = m_staticals_first;

        while(b_next)
        {
          callback(*a_next->object,*b_next->object);

          b_next = b_next->next;
        }


      a_next = a_next->next;
    }
}




void
space::
update_objects(object_node*  nd) noexcept
{
  auto  next = nd;

    while(next)
    {
      next->object->save_area();

      next->object->update();

      next = next->next;
    }
}


void
space::
update() noexcept
{
  update_objects( m_staticals_first);
  update_objects(m_dynamicals_first);
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
  render_objects( m_staticals_first,dst);
  render_objects(m_dynamicals_first,dst);
}


void
default_detection(object&  a, object&  b) noexcept
{
    if(!spaces::area::test_collision(a.get_area(),b.get_area()))
    {
      return;
    }


  auto&     fixed_obj = a.is_fixed()? a:b;
  auto&  nonfixed_obj = a.is_fixed()? b:a;

  auto&  target_area =    fixed_obj.get_area();
  auto&   saved_area = nonfixed_obj.get_saved_area();

    if(saved_area.left >= target_area.right)
    {
      nonfixed_obj.set_left(target_area.right);
    }

  else
    if(saved_area.right <= target_area.left)
    {
      nonfixed_obj.set_right(target_area.left);
    }

  else
    if(saved_area.top >= target_area.bottom)
    {
      nonfixed_obj.set_top(target_area.bottom);
    }

  else
    if(saved_area.bottom <= target_area.top)
    {
      nonfixed_obj.set_bottom(target_area.top);
    }


/*
    if(nonfixed_obj.is_moved_to_down())
    {

      nonfixed_obj.set_y_kinetic_energy(0);
    }

  else
    if(nonfixed_obj.is_moved_to_left())
    {
      nonfixed_obj.set_left(fixed_obj.get_area().right);

      nonfixed_obj.set_x_kinetic_energy(0);
    }

  else
    if(nonfixed_obj.is_moved_to_right())
    {
      nonfixed_obj.set_right(fixed_obj.get_area().left);

      nonfixed_obj.set_x_kinetic_energy(0);
    }
*/
}




}}




