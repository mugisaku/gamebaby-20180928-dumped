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
  gbstd::string  m_name;

  real_point       m_base_point;
  real_point  m_last_base_point;

  point  m_offset;

  int  m_width =0;
  int  m_height=0;

  real_point  m_kinetic_energy;

  area  m_area;
  area  m_last_area;

  uint32_t*  m_dieing_counter=nullptr;

  uint32_t  m_rendering_counter=0;

  struct flags{
    static constexpr int  frozen = 1;
    static constexpr int visible = 2;
  };


  int  m_state=flags::visible;

  object&    set_flag(int  f) noexcept{  m_state |=  f;  return *this;}
  object&  unset_flag(int  f) noexcept{  m_state &= ~f;  return *this;}

public:
  object() noexcept{}
  object(rectangle  rect) noexcept: m_base_point(rect.x,rect.y),
  m_width(rect.w),
  m_height(rect.h){}


/*
  object(      object&&  rhs) noexcept{*this = std::move(rhs);}
  object(const object&   rhs) noexcept{*this = rhs;}


  object&  operator=(      object&&  rhs) noexcept{assign(std::move(rhs));}
  object&  operator=(const object&   rhs) noexcept{assign(rhs);}


  object&  assign(      object&&  rhs) noexcept;
  object&  assign(const object&   rhs) noexcept;
*/


  void                  set_name(gbstd::string_view  sv)       noexcept{       m_name = sv;}
  const gbstd::string&  get_name(                      ) const noexcept{return m_name     ;}

  bool    is_alive() const noexcept{return m_dieing_counter;}
  bool   is_frozen() const noexcept{return m_state&flags::frozen;}
  bool  is_visible() const noexcept{return m_state&flags::visible;}

  object&      show() noexcept{return   set_flag(flags::visible);}
  object&      hide() noexcept{return unset_flag(flags::visible);}
  object&    freeze() noexcept{return   set_flag(flags::frozen);}
  object&  unfreeze() noexcept{return unset_flag(flags::frozen);}

  void  be_alive(uint32_t&  v) noexcept;
  void  die() noexcept;

  const uint32_t&  get_rendering_counter(      ) const noexcept{return m_rendering_counter     ;}
  void             add_rendering_counter(int  n)       noexcept{       m_rendering_counter += n;}

  const area&  get_area() const noexcept{return m_area;}
  const area&  get_last_area() const noexcept{return m_last_area;}

  void  set_base_point(real_point  new_pt) noexcept{m_base_point = new_pt;}
  void  set_base_point(double  x, double  y) noexcept{m_base_point = real_point(x,y);}
  void  set_base_point_x(double  x) noexcept{m_base_point.x = x;}
  void  set_base_point_y(double  y) noexcept{m_base_point.y = y;}

  void  add_base_point_x(double  x) noexcept{m_base_point.x += x;}
  void  add_base_point_y(double  y) noexcept{m_base_point.y += y;}

  void  add_base_point(double  x, double  y) noexcept{m_base_point += real_point(x,y);}
  void  add_base_point(real_point  pt) noexcept{m_base_point += pt;}

  const real_point&  get_base_point() const noexcept{return m_base_point;}
  const real_point&  get_last_base_point() const noexcept{return m_last_base_point;}

  void  update_base_point() noexcept;

  bool  did_move_to_left()  const noexcept{return m_area.left   < m_last_area.left;}
  bool  did_move_to_right() const noexcept{return m_area.right  > m_last_area.right;}
  bool  did_move_to_up()    const noexcept{return m_area.top    < m_last_area.top;}
  bool  did_move_to_down()  const noexcept{return m_area.bottom > m_last_area.bottom;}

  point  get_point() const noexcept;

  point  get_left_point()   const noexcept{return point(m_area.left,m_base_point.y);}
  point  get_right_point()  const noexcept{return point(m_area.right,m_base_point.y);}
  point  get_top_point()    const noexcept{return point(m_base_point.x,m_area.top);}
  point  get_bottom_point() const noexcept{return point(m_base_point.x,m_area.bottom);}

  const point&  get_offset() const noexcept{return m_offset;}

  void  set_offset(point  off) noexcept{m_offset = off;}
  void  set_offset(int  x, int  y) noexcept{m_offset = point(x,y);}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  void  set_width( int  v) noexcept{m_width  = v;}
  void  set_height(int  v) noexcept{m_height = v;}


        real_point&  get_kinetic_energy()       noexcept{return m_kinetic_energy;}
  const real_point&  get_kinetic_energy() const noexcept{return m_kinetic_energy;}

  void  set_kinetic_energy(double  x, double  y) noexcept{m_kinetic_energy  = real_point(x,y);}
  void  add_kinetic_energy(double  x, double  y) noexcept{m_kinetic_energy += real_point(x,y);}

  void  set_kinetic_energy(real_point  pt) noexcept{m_kinetic_energy  = pt;}
  void  add_kinetic_energy(real_point  pt) noexcept{m_kinetic_energy += pt;}

  void  add_kinetic_energy_x(double  v) noexcept{m_kinetic_energy.x += v;}
  void  add_kinetic_energy_y(double  v) noexcept{m_kinetic_energy.y += v;}

  void  set_kinetic_energy_x(double  v) noexcept{m_kinetic_energy.x = v;}
  void  set_kinetic_energy_y(double  v) noexcept{m_kinetic_energy.y = v;}


  void  set_left(  int  v) noexcept;
  void  set_right( int  v) noexcept;
  void  set_top(   int  v) noexcept;
  void  set_bottom(int  v) noexcept;

  void  update_area() noexcept;

  rectangle  get_rectangle() const noexcept;


  void  do_when_collided(object&  other_side, positions::position  position) noexcept{}

  virtual void  update_core() noexcept{}
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

  const image&  get_image(                 ) const noexcept{return *m_image       ;}
  void          set_image(const image&  img)       noexcept{        m_image = &img;}


  const rectangle&  get_image_rectangle(               ) const noexcept{return m_image_rectangle       ;}
  void              set_image_rectangle(rectangle  rect)       noexcept{       m_image_rectangle = rect;}

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

  text_style  m_style = styles::a_white_based_text_style;

  point  m_rendering_offset;

public:
  text_object() noexcept{}

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
  struct element{
    T*  data;

    void  (*deleter)(T*  ptr);
  };


  std::vector<element>  m_pool_list;
  std::vector<element>  m_main_list;
  std::vector<element>  m_keep_list;

  std::vector<element>*  m_current_list=&m_main_list;

  uint32_t  m_dieing_counter=0;

  static void  default_deleter(T*  ptr) noexcept{delete ptr;}

public:
  const std::vector<element>&  get_pool_list() const noexcept{return m_pool_list;}
  const std::vector<element>&  get_main_list() const noexcept{return m_main_list;}

  template<typename  U>
  void  process(const U&  t) noexcept
  {
      for(auto&  e: m_main_list)
      {
        t(*e.data);
      }
  }

  void  append(T&  o) noexcept
  {
      if(!o.is_alive())
      {
        m_current_list->emplace_back(element{&o,nullptr});

        o.be_alive(m_dieing_counter);

        o.update_area();
      }
  }

  void  append_with_deleter(T&  o, void  (*deleter)(T*  ptr)=&default_deleter) noexcept
  {
      if(!o.is_alive())
      {
        m_current_list->emplace_back(element{&o,deleter});

        o.be_alive(m_dieing_counter);

        o.update_area();
      }
  }

  void  remove_all() noexcept
  {
      for(auto&  e: m_pool_list)
      {
        e.data->die();

          if(e.deleter)
          {
            e.deleter(e.data);
          }
      }


      for(auto&  e: m_main_list)
      {
        e.data->die();

          if(e.deleter)
          {
            e.deleter(e.data);
          }
      }


    m_dieing_counter = 0;

    m_pool_list.clear();
    m_main_list.clear();
  }

  void  clean_dead_object() noexcept
  {
      if(m_dieing_counter)
      {
        m_keep_list.clear();

          for(auto&  e: m_main_list)
          {
              if(e.data->is_alive())
              {
                m_keep_list.emplace_back(e);
              }

            else
              {
                  if(e.deleter)
                  {
                    e.deleter(e.data);
                  }
              }
          }


        std::swap(m_main_list,m_keep_list);

        m_dieing_counter = 0;
      }
  }

  void  process_collision(T&  a, T&  b) noexcept
  {
    auto&    last_area = a.get_last_area();
    auto&  target_area = b.get_area();

    position  pos;

      if(last_area.left >= target_area.right)
      {
        pos = position::left;
      }

    else
      if(last_area.right <= target_area.left)
      {
        pos = position::right;
      }

    else
      if(last_area.top >= target_area.bottom)
      {
        pos = position::top;
      }

    else
      if(last_area.bottom <= target_area.top)
      {
        pos = position::bottom;
      }

    else
      {
        pos = position::none;
      }


    a.do_when_collided(b,pos);
    b.do_when_collided(a,get_opposite(pos));
  }


  void  detect_collision() noexcept
  {
    m_current_list = &m_pool_list;

      for(auto&  e: m_pool_list)
      {
        m_main_list.emplace_back(e);
      }


    m_pool_list.clear();

      if(m_main_list.size() >= 2)
      {
        auto  a_current = m_main_list.begin();
        auto  b_base    = a_current+1;

        auto  end = m_main_list.end();

          while(b_base < end)
          {
            auto  b_current = b_base++;

              while(b_current < end)
              {
                auto&  a = *static_cast<T*>(a_current->data);
                auto&  b = *static_cast<T*>(b_current->data);

                  if(area::test_collision(a.get_area(),b.get_area()))
                  {
                    process_collision(a,b);
                  }


                ++b_current;
              }


            ++a_current;
          }
      }


    m_current_list = &m_main_list;
  }

  void  update() noexcept
  {
    m_current_list = &m_pool_list;

      for(auto&  e: m_pool_list)
      {
        m_main_list.emplace_back(e);
      }


    m_pool_list.clear();

      for(auto&  e: m_main_list)
      {
          if(e.data->is_alive())
          {
              if(!e.data->is_frozen())
              {
                e.data->update_base_point();

                e.data->update_core();
              }
          }
      }


    m_current_list = &m_main_list;
  }

  void  render(point  offset, image_cursor  cur) noexcept
  {
    clean_dead_object();

    auto  it     = m_main_list.rbegin();
    auto  it_end = m_main_list.rend();

      while(it != it_end)
      {
        auto&  e = *it++;

          if(e.data->is_visible())
          {
            e.data->update_graphics();

            e.data->render(offset,cur);

            e.data->add_rendering_counter(1);
          }
      }
  }

};


}


}




#endif




