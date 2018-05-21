#ifndef space_HPP
#define space_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"


namespace gbstd{
namespace spaces{


class space;


class
environment
{
  double  m_gravitation=0;
  double  m_fluid_viscosity=0;

  real_point  m_fluid_kinetic_energy;

public:
  double  get_gravitation(         ) const noexcept{return m_gravitation    ;}
  void    set_gravitation(double  v)       noexcept{       m_gravitation = v;}

  double  get_fluid_viscosity(         ) const noexcept{return m_fluid_viscosity    ;}
  void    set_fluid_viscosity(double  v)       noexcept{       m_fluid_viscosity = v;}

  real_point  get_fluid_kinetic_energy(              ) const noexcept{return m_fluid_kinetic_energy      ;}
  void        set_fluid_kinetic_energy(real_point  pt)       noexcept{       m_fluid_kinetic_energy  = pt;}
  void        add_fluid_kinetic_energy(real_point  pt)       noexcept{       m_fluid_kinetic_energy += pt;}

};


struct
area
{
  int     top=0;
  int    left=0;
  int   right=0;
  int  bottom=0;


  static bool  test_x_collision(const area&  a, const area&  b) noexcept;
  static bool  test_y_collision(const area&  a, const area&  b) noexcept;
  static bool  test_collision(  const area&  a, const area&  b) noexcept;

};


class
body
{
  real_point  m_base_point;

  point  m_offset;

  int  m_width =0;
  int  m_height=0;

  area  m_saved_area;
  area        m_area;

public:
  body() noexcept{}
  body(rectangle  rect) noexcept: m_base_point(rect.x,rect.y),
  m_width(rect.w),
  m_height(rect.h){}


  void  save_area() noexcept{m_saved_area = m_area;}

  const area&  get_area()       const noexcept{return m_area;}
  const area&  get_saved_area() const noexcept{return m_saved_area;}

  void  set_base_point(real_point  new_pt) noexcept;
  void  add_base_point(real_point      pt) noexcept;

  point  get_point() const noexcept;

  const real_point&  get_base_point() const noexcept{return m_base_point;}

  const point&  get_offset() noexcept{return m_offset;}

  void  set_offset(point  off) noexcept{m_offset = off;}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  bool  is_moved_to_up()    const noexcept{return m_area.top  < m_saved_area.top;}
  bool  is_moved_to_down()  const noexcept{return m_area.top  > m_saved_area.top;}
  bool  is_moved_to_left()  const noexcept{return m_area.left < m_saved_area.left;}
  bool  is_moved_to_right() const noexcept{return m_area.left > m_saved_area.left;}

  void  set_left(  int  v) noexcept;
  void  set_right( int  v) noexcept;
  void  set_top(   int  v) noexcept;
  void  set_bottom(int  v) noexcept;

  void  update() noexcept;

  rectangle  get_rectangle() const noexcept;

  void  print() const noexcept;

};


class
object: public body
{
protected:
  gbstd::string  m_name;

  space*  m_space=nullptr;

  real_point  m_kinetic_energy;

  int  m_mass=0;
  int  m_mark=0;

  double  m_energy_refection=0;

  enum class state{
    fixed,
    landing,
    floating,

  } m_state=state::floating;

public:
  object() noexcept{}
  object(rectangle  rect) noexcept: body(rect){}

  virtual ~object(){}

  void    set_space(space&  sp)       noexcept{        m_space = &sp;}
  space&  get_space(          ) const noexcept{return *m_space      ;}

  real_point  get_kinetic_energy(              ) const noexcept{return m_kinetic_energy      ;}
  void        set_kinetic_energy(real_point  pt)       noexcept{       m_kinetic_energy  = pt;}
  void        add_kinetic_energy(real_point  pt)       noexcept{       m_kinetic_energy += pt;}

  void  set_x_kinetic_energy(double  v) noexcept{m_kinetic_energy.x = v;}
  void  set_y_kinetic_energy(double  v) noexcept{m_kinetic_energy.y = v;}


  bool  is_fixed()    const noexcept{return m_state == state::fixed;}
  bool  is_landing()  const noexcept{return m_state == state::landing;}
  bool  is_floating() const noexcept{return m_state == state::floating;}

  object&  be_fixed()    noexcept{  m_state =    state::fixed;  return *this;}
  object&  be_landing()  noexcept{  m_state =  state::landing;  return *this;}
  object&  be_floating() noexcept{  m_state = state::floating;  return *this;}

  void  set_mark(int  v)       noexcept{       m_mark = v;}
  int   get_mark(      ) const noexcept{return m_mark    ;}


  virtual void  update() noexcept;

  virtual void  render(image&  dst) const noexcept{}

  void  print() const noexcept;

};


class
rectangle_object: public object
{
  color  m_color;

public:
  rectangle_object(rectangle  rect, colors::color  color) noexcept:
  object(rect),
  m_color(color){}

  void  render(image&  dst) const noexcept override
  {
    auto  rect = get_rectangle();

    dst.draw_rectangle_safely(m_color,rect.x,rect.y,rect.w,rect.h);
  }

};


class
space
{
  struct object_node;

  std::vector<space*>  m_child_spaces;

  object_node*  m_dynamicals_first=nullptr;
  object_node*  m_dynamicals_last =nullptr;

  object_node*  m_staticals_first=nullptr;
  object_node*  m_staticals_last =nullptr;

  object_node*  m_trash=nullptr;

  object_node*  pop_node() noexcept;
  void          push_node(object_node*  nd) noexcept;


  space*  m_parent_space=nullptr;

  gbstd::string  m_name;

  point  m_absolute_point;
  point  m_relative_point;

  int  m_width =0;
  int  m_height=0;

  int  m_right_position =0;
  int  m_bottom_position=0;

  environment  m_environment;

  void  update_objects(object_node*  nd) noexcept;

  void  render_objects(object_node*  nd, image&  dst) const noexcept;

  void  append_object(object&  o, object_node*&  first, object_node*&  last) noexcept;

public:
  void  append_dynamical_object(object&  o) noexcept;
  void  append_statical_object(object&  o) noexcept;

  void  append_child(space&  sp, int  x, int  y) noexcept;


  const gbstd::string&  get_name() const noexcept{return m_name;}

  const point&  get_absolute_point() const noexcept{return m_absolute_point;}
  const point&  get_relative_point() const noexcept{return m_relative_point;}

  environment&  get_environment() noexcept{return m_environment;}

  void  reform() noexcept;

  void  detect_dd_collision(void  (*callback)(object&  a, object&  b)) noexcept;
  void  detect_ds_collision(void  (*callback)(object&  a, object&  b)) noexcept;

  virtual void  update() noexcept;

  virtual void  render(image&  dst) const noexcept;

};


void  default_detection(object&  a, object&  b) noexcept;


}


}




#endif




