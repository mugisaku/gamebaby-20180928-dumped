#ifndef space_HPP
#define space_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include"libgbstd/area.hpp"
#include"libgbstd/position.hpp"


namespace gbstd{
namespace spaces{


class object;


class
object
{
  real_point  m_base_point;

  point  m_offset;

  int  m_width =0;
  int  m_height=0;

  real_point  m_kinetic_energy;

  area  m_area;
  area  m_saved_area;

  struct flags{
    static constexpr int   alive = 1;
    static constexpr int  frozen = 2;
    static constexpr int visible = 4;
  };

  int  m_state=0;

  object&    set_flag(int  f) noexcept{  m_state |=  f;  return *this;}
  object&  unset_flag(int  f) noexcept{  m_state &= ~f;  return *this;}

public:
  object() noexcept{}
  object(rectangle  rect) noexcept: m_base_point(rect.x,rect.y),
  m_width(rect.w),
  m_height(rect.h){}


  bool    is_alive() const noexcept{return m_state&flags::alive;}
  bool   is_frozen() const noexcept{return m_state&flags::frozen;}
  bool  is_visible() const noexcept{return m_state&flags::visible;}

  object&      show() noexcept{return   set_flag(flags::visible);}
  object&      hide() noexcept{return unset_flag(flags::visible);}
  object&    freeze() noexcept{return   set_flag(flags::frozen);}
  object&  unfreeze() noexcept{return unset_flag(flags::frozen);}
  object&  be_alive() noexcept{return   set_flag(flags::alive);}
  object&       die() noexcept{return unset_flag(flags::alive);}

  void  save_area() noexcept{m_saved_area = m_area;}

  const area&  get_area()       const noexcept{return m_area;}
  const area&  get_saved_area() const noexcept{return m_saved_area;}

  void  set_base_point(real_point  new_pt) noexcept{m_base_point = new_pt;}
  void  set_base_point(double  x, double  y) noexcept{m_base_point = real_point(x,y);}
  void  set_base_point_x(double  x) noexcept{m_base_point.x = x;}
  void  set_base_point_y(double  y) noexcept{m_base_point.y = y;}

  void  add_base_point_x(double  x) noexcept{m_base_point.x += x;}
  void  add_base_point_y(double  y) noexcept{m_base_point.y += y;}

  void  add_base_point(double  x, double  y) noexcept{m_base_point += real_point(x,y);}
  void  add_base_point(real_point  pt) noexcept{m_base_point += pt;}
  const real_point&  get_base_point() const noexcept{return m_base_point;}

  point  get_point() const noexcept;

  point  get_left_point()   const noexcept{return point(m_area.left,m_base_point.y);}
  point  get_right_point()  const noexcept{return point(m_area.right,m_base_point.y);}
  point  get_top_point()    const noexcept{return point(m_base_point.x,m_area.top);}
  point  get_bottom_point() const noexcept{return point(m_base_point.x,m_area.bottom);}

  const point&  get_offset() const noexcept{return m_offset;}

  void  set_offset(point  off) noexcept{m_offset = off;}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  void  set_width( int  v) noexcept{m_width  = v;}
  void  set_height(int  v) noexcept{m_height = v;}


        real_point&  get_kinetic_energy()       noexcept{return m_kinetic_energy;}
  const real_point&  get_kinetic_energy() const noexcept{return m_kinetic_energy;}

  void  set_kinetic_energy(real_point  pt) noexcept{m_kinetic_energy  = pt;}
  void  add_kinetic_energy(real_point  pt) noexcept{m_kinetic_energy += pt;}

  void  add_kinetic_energy_x(double  v) noexcept{m_kinetic_energy.x += v;}
  void  add_kinetic_energy_y(double  v) noexcept{m_kinetic_energy.y += v;}

  void  set_kinetic_energy_x(double  v) noexcept{m_kinetic_energy.x = v;}
  void  set_kinetic_energy_y(double  v) noexcept{m_kinetic_energy.y = v;}


  bool  is_moved_to_up()    const noexcept{return m_area.top  < m_saved_area.top;}
  bool  is_moved_to_down()  const noexcept{return m_area.top  > m_saved_area.top;}
  bool  is_moved_to_left()  const noexcept{return m_area.left < m_saved_area.left;}
  bool  is_moved_to_right() const noexcept{return m_area.left > m_saved_area.left;}

  void  set_left(  int  v) noexcept;
  void  set_right( int  v) noexcept;
  void  set_top(   int  v) noexcept;
  void  set_bottom(int  v) noexcept;

  void  update_area() noexcept;

  rectangle  get_rectangle() const noexcept;


  void  do_when_collided(object&  other_side, positions::position  position) noexcept{}

  virtual void  update_core() noexcept;
  virtual void  update_graphics() noexcept{}

  virtual void  render(point  offset, image_cursor  cur) noexcept{}

  virtual void  print() const noexcept;

};


class
image_object: public object
{
  const image*  m_image=nullptr;

  rectangle  m_image_rectangle;

  point  m_rendering_offset;

public:
  image_object() noexcept{}

  image_object(const image&  image, rectangle  image_rect, point  rend_off) noexcept:
  object(rectangle(point(),image_rect.w,image_rect.h)),
  m_image(&image), m_image_rectangle(image_rect), m_rendering_offset(rend_off){}

  const image&  get_image(           ) const noexcept{return *m_image       ;}
  void          set_image(image&  img)       noexcept{        m_image = &img;}

  void  set_image_rectangle(rectangle  rect) noexcept{m_image_rectangle = rect;}

  void  set_rendering_offset(point  off) noexcept{m_rendering_offset = off;}

  const point&  get_rendering_offset() const noexcept{return m_rendering_offset;}

  void  render(point  offset, image_cursor  cur) noexcept override
  {
      if(m_image)
      {
        images::overlay(*m_image,m_image_rectangle,cur+(point(get_base_point())+m_rendering_offset-offset));
      }
  }

};


class
text_object: public object
{
  gbstd::u16string  m_string;

  text_style  m_style;

  point  m_rendering_offset;

public:
  text_object(gbstd::string_view  sv, const text_style&  style) noexcept:
  m_style(style){set_string(sv);}

  text_object(gbstd::u16string_view  sv, const text_style&  style) noexcept:
  m_style(style){set_string(sv);}

  void  set_string(gbstd::string_view     sv) noexcept;
  void  set_string(gbstd::u16string_view  sv) noexcept;

  void  align_left(  ) noexcept;
  void  align_right( ) noexcept;
  void  align_center() noexcept;

  void  render(point  offset, image_cursor  cur) noexcept override;

};


template<typename  T>
class
space
{
  std::vector<T*>          m_object_list;
  std::vector<T*>  m_updated_object_list;

public:
  void  append(T&  o) noexcept
  {
    m_object_list.emplace_back(&o);

    o.be_alive();

    o.update_area();
  }

  void  remove_all() noexcept
  {
    m_object_list.clear();
  }

  void  process_collision(T&  a, T&  b) noexcept
  {
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


  void  detect_collision() noexcept
  {
       if(m_object_list.size() >= 2)
       {
         auto  a_current = m_object_list.begin();
         auto  b_base    = a_current+1;

         auto  end = m_object_list.end();

           while(b_base < end)
           {
             auto  b_current = b_base++;

               while(b_current < end)
               {
                 auto&  a = *static_cast<T*>(*a_current);
                 auto&  b = *static_cast<T*>(*b_current);

                   if(area::test_collision(a.get_area(),b.get_area()))
                   {
                     process_collision(a,b);
                   }


                 ++b_current;
               }


             ++a_current;
           }
       }
  }

  void  update() noexcept
  {
    m_updated_object_list.clear();

      for(auto  o: m_object_list)
      {
          if(o->is_alive())
          {
            o->update_core();

            m_updated_object_list.emplace_back(o);
          }
      }


    m_object_list.clear();

    std::swap(m_object_list,m_updated_object_list);
  }

  void  render(point  offset, image_cursor  cur) const noexcept
  {
      for(auto  o: m_object_list)
      {
        o->update_graphics();

        o->render(offset,cur);
      }
  }

};


}


}




#endif




