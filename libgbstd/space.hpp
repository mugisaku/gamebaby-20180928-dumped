#ifndef space_HPP
#define space_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include"libgbstd/board.hpp"


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

  boards::area  m_area;
  boards::area  m_saved_area;

public:
  body() noexcept{}
  body(rectangle  rect) noexcept: m_base_point(rect.x,rect.y),
  m_width(rect.w),
  m_height(rect.h){}


  void  save_area() noexcept{m_saved_area = m_area;}

  const boards::area&  get_area()       const noexcept{return m_area;}
  const boards::area&  get_saved_area() const noexcept{return m_saved_area;}

  void  set_base_point(real_point  new_pt) noexcept;
  void  set_base_point_x(double  x) noexcept{m_base_point.x = x;}
  void  set_base_point_y(double  y) noexcept{m_base_point.y = y;}

  void  add_base_point(real_point      pt) noexcept;
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


class
object: public body
{
protected:
  gbstd::string  m_name;

  space*  m_space=nullptr;

  uint32_t  m_kind_code=0;

  bool  m_needed_to_remove=false;

  boards::square*            m_current_square=nullptr;
  boards::square*     m_left_contacted_square=nullptr;
  boards::square*    m_right_contacted_square=nullptr;
  boards::square*       m_up_contacted_square=nullptr;
  boards::square*     m_down_contacted_square=nullptr;

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


  void                   set_current_square(boards::square*  sq)       noexcept{       m_current_square = sq;}
  const boards::square*  get_current_square(                   ) const noexcept{return m_current_square     ;}

  void  set_left_contacted_square(boards::square*  sq) noexcept{m_left_contacted_square = sq;}
        boards::square*  get_left_contacted_square()       noexcept{return m_left_contacted_square;}
  const boards::square*  get_left_contacted_square() const noexcept{return m_left_contacted_square;}

  void  set_right_contacted_square(boards::square*  sq) noexcept{m_right_contacted_square = sq;}
        boards::square*  get_right_contacted_square()       noexcept{return m_right_contacted_square;}
  const boards::square*  get_right_contacted_square() const noexcept{return m_right_contacted_square;}

  void  set_up_contacted_square(boards::square*  sq) noexcept{m_up_contacted_square = sq;}
        boards::square*  get_up_contacted_square()       noexcept{return m_up_contacted_square;}
  const boards::square*  get_up_contacted_square() const noexcept{return m_up_contacted_square;}

  void  set_down_contacted_square(boards::square*  sq) noexcept{m_down_contacted_square = sq;}
        boards::square*  get_down_contacted_square()       noexcept{return m_down_contacted_square;}
  const boards::square*  get_down_contacted_square() const noexcept{return m_down_contacted_square;}


  bool  is_needed_to_remove() const noexcept{return m_needed_to_remove;}
  void    need_to_remove() noexcept{m_needed_to_remove =  true;}
  void  unneed_to_remove() noexcept{m_needed_to_remove = false;}

  virtual void  do_when_collided( object&  other_side, positions::position  position) noexcept{}
  virtual void  do_when_entered(boards::square&  square) noexcept{}
  virtual void  do_when_removed() noexcept{}

  virtual void  update_core() noexcept;
  virtual void  update_graphics() noexcept{}

  virtual void  render(point  offset, image&  dst) noexcept{}

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

  void  render(point  offset, image&  dst) noexcept override
  {
      if(m_image)
      {
        images::overlay(*m_image,m_image_rectangle,dst,get_base_point()+m_rendering_offset-offset);
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

  void  render(point  offset, image&  dst) noexcept override;

};


class
space
{
  struct object_node;

  object_node*  m_object_list=nullptr;

  object_node*  m_trash=nullptr;

  object_node*  pop_node_from_trash() noexcept;
  void          push_node_to_trash(object_node*  nd) noexcept;


  int  m_width =0;
  int  m_height=0;

  environment  m_environment;

  void  check_collision(object&  a, object&  b) noexcept;

public:
 ~space(){empty_trash();}

  void  append_object(object&  o, bool  use_env=false) noexcept;

  void  remove_all_object() noexcept;

  void  empty_trash() noexcept;

  environment&  get_environment() noexcept{return m_environment;}

  void  detect_collision() noexcept;
  void  detect_collision(boards::board&  board) noexcept;

  void  update() noexcept;

  void  render(point  offset, image&  dst) const noexcept;

};


}


}




#endif




