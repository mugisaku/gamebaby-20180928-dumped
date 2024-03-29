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
object_id
{
  int  m_major=0;
  int  m_minor=0;

public:
  constexpr object_id() noexcept{}
  constexpr object_id(int  maj, int  min) noexcept: m_major(maj), m_minor(min){}

  constexpr int  get_major() const noexcept{return m_major;}
  constexpr int  get_minor() const noexcept{return m_minor;}

  void  set_major(int  n) noexcept{m_major = n;}
  void  set_minor(int  n) noexcept{m_minor = n;}

};


class
object: public object_id
{
  const char*  m_name;

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


  void                set_name(const char*  name)       noexcept{m_name = name;}
  gbstd::string_view  get_name(                 ) const noexcept{return gbstd::string_view(m_name);}

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

  bool  m_visible=true;

  static void  default_deleter(T*  ptr) noexcept{delete ptr;}

public:
  bool  is_visible() const noexcept{return m_visible;}

  void  show() noexcept{m_visible =  true;}
  void  hide() noexcept{m_visible = false;}

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


  void  append(T&  o) noexcept;

  void  append_with_deleter(T&  o, void  (*deleter)(T*  ptr)=&default_deleter) noexcept;

  void  remove_all() noexcept;

  void  clean_dead_object() noexcept;

  void  process_collision(T&  a, T&  b) noexcept;

  void  detect_collision() noexcept;

  void  update() noexcept;

  void  render(point  offset, image_cursor  cur) noexcept;

};


#include"libgbstd/spaces/space.tcpp"


}


}




#endif




