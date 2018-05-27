#ifndef space_HPP
#define space_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"


namespace gbstd{
namespace spaces{


class object;
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
  int  m_mass=0;

  double  m_energy_refection=0;

  real_point  m_base_point;

  point  m_offset;

  int  m_width =0;
  int  m_height=0;

  const environment*  m_environment=nullptr;

  real_point  m_kinetic_energy;

  area  m_area;
  area  m_saved_area;

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
  const real_point&  get_base_point() const noexcept{return m_base_point;}

  point  get_point() const noexcept;

  const point&  get_offset() const noexcept{return m_offset;}

  void  set_offset(point  off) noexcept{m_offset = off;}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  void  set_width( int  v) noexcept{m_width  = v;}
  void  set_height(int  v) noexcept{m_height = v;}

  void                set_environment(const environment*  env)       noexcept{       m_environment = env;}
  const environment*  get_environment(                       ) const noexcept{return m_environment      ;}

  bool  is_kinetic() const noexcept{return m_environment;}

  real_point  get_kinetic_energy(              ) const noexcept{return m_kinetic_energy      ;}
  void        set_kinetic_energy(real_point  pt)       noexcept{       m_kinetic_energy  = pt;}
  void        add_kinetic_energy(real_point  pt)       noexcept{       m_kinetic_energy += pt;}

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

  void  update() noexcept;

  rectangle  get_rectangle() const noexcept;

  void  print() const noexcept;

};


enum class
position
{
     top,
    left,
   right,
  bottom,

};


constexpr position
get_opposite(position  pos) noexcept
{
  return( (pos == position::top  )? position::bottom
         :(pos == position::left )? position::right
         :(pos == position::right)? position::left
         :                          position::top);
}


class
object: public body
{
protected:
  gbstd::string  m_name;

  space*  m_space=nullptr;

  uint32_t  m_kind_code=0;

  bool  m_needed_to_remove=false;

public:
  object() noexcept{}
  object(rectangle  rect) noexcept: body(rect){}

  virtual ~object(){}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  uint32_t  get_kind_code(              ) const noexcept{return m_kind_code       ;}
  void      set_kind_code(uint32_t  code)       noexcept{       m_kind_code = code;}

  space*  get_space() const noexcept{return m_space;}

  void    set_space(space&  sp) noexcept{m_space = &sp;}
  void  unset_space(          ) noexcept{m_space = nullptr;}

  bool  is_needed_to_remove() const noexcept{return m_needed_to_remove;}
  void    need_to_remove() noexcept{m_needed_to_remove =  true;}
  void  unneed_to_remove() noexcept{m_needed_to_remove = false;}

  virtual void  do_when_collided( object&  other_side, spaces::position  position) noexcept{}
  virtual void  do_when_removed() noexcept{}

  virtual void  update_core() noexcept;
  virtual void  update_graphics() noexcept{}

  virtual void  render(image&  dst) noexcept{}

  virtual void  print() const noexcept;

};


class
rectangle_object: public object
{
  color  m_color;

public:
  rectangle_object(rectangle  rect, colors::color  color) noexcept:
  object(rect),
  m_color(color){}

  void  set_color(color  new_color) noexcept{m_color = new_color;}

  void  render(image&  dst) noexcept override
  {
    auto  rect = get_rectangle();

    dst.fill_rectangle(m_color,rect.x,rect.y,rect.w,rect.h);
  }

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

  void  render(image&  dst) noexcept override
  {
      if(m_image)
      {
        images::overlay(*m_image,m_image_rectangle,dst,get_base_point()+m_rendering_offset);
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

  void  render(image&  dst) noexcept override;

};


class
space
{
  struct object_node;

  object_node*  m_object_list=nullptr;
  object_node*  m_kinetic_object_list=nullptr;

  object_node*  m_trash=nullptr;

  object_node*  pop_node_from_trash() noexcept;
  void          push_node_to_trash(object_node*  nd) noexcept;


  int  m_width =0;
  int  m_height=0;

  environment  m_environment;

  void  update_objects(object_node*&  list) noexcept;

  void  render_objects(object_node*  list, image&  dst) const noexcept;

  void  append_object(object&  o, object_node*&  ls) noexcept;

  void  check_collision(object&  a, object&  b) noexcept;

public:
 ~space(){empty_trash();}

  void  append_object(        object&  o) noexcept;
  void  append_kinetic_object(object&  o) noexcept;

  void  remove_all_object() noexcept;

  void  empty_trash() noexcept;

  environment&  get_environment() noexcept{return m_environment;}

  void  detect_collision() noexcept;

  void  update() noexcept;

  void  render(image&  dst) const noexcept;

};


}


}




#endif




