#ifndef board_HPP
#define board_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/position.hpp"
#include"libgbstd/area.hpp"


namespace gbstd{




class
gate
{
  int  m_bits=0;

public:
  constexpr gate(int  bits=0) noexcept: m_bits(bits){}

  constexpr bool  test_left()   const noexcept{return m_bits&0b01000;}
  constexpr bool  test_right()  const noexcept{return m_bits&0b00100;}
  constexpr bool  test_top()    const noexcept{return m_bits&0b00010;}
  constexpr bool  test_bottom() const noexcept{return m_bits&0b00001;}

  constexpr gate  operator|(gate  rhs) const noexcept{return gate(m_bits|rhs.m_bits);}
  constexpr gate  operator~() const noexcept{return gate(~m_bits);}

  static constexpr gate  get_left()   noexcept{return gate(0b1000);}
  static constexpr gate  get_right()  noexcept{return gate(0b0100);}
  static constexpr gate  get_top()    noexcept{return gate(0b0010);}
  static constexpr gate  get_bottom() noexcept{return gate(0b0001);}

  static constexpr gate  get_all()  noexcept{return gate(0b1111);}
  static constexpr gate  get_none() noexcept{return gate(0b0000);}

  static constexpr gate  get_left_and_right() noexcept{return gate(0b1100);}
  static constexpr gate  get_top_and_bottom() noexcept{return gate(0b0011);}

};




namespace spaces{
class object;
}


namespace boards{




struct
links
{
  static constexpr int     up = 0;
  static constexpr int   left = 1;
  static constexpr int  right = 2;
  static constexpr int   down = 3;

  static constexpr int   upper_left = 4;
  static constexpr int  upper_right = 5;
  static constexpr int   lower_left = 6;
  static constexpr int  lower_right = 7;


};


class square;
class piece;


class
square_data
{
  point  m_image_point;

  gate  m_gate;

public:
  constexpr square_data(int  x=0, int  y=0, gate  g=gate()) noexcept: m_image_point(x,y), m_gate(g){}

  constexpr point  get_image_point(         ) const noexcept{return m_image_point     ;}
  void             set_image_point(point  pt)       noexcept{       m_image_point = pt;}

  constexpr gate  get_gate(       ) const noexcept{return m_gate    ;}
  void            set_gate(gate  g)       noexcept{       m_gate = g;}

};


class
square
{
  point  m_index;

  area  m_area;

  square*  m_link_table[8];

  square_data*  m_data=nullptr;

public:
  const point&  get_index(         ) const noexcept{return m_index     ;}
  void          set_index(point  pt)       noexcept{       m_index = pt;}

  const area&  get_area(          ) const noexcept{return m_area       ;}
  void         set_area(area  area)       noexcept{       m_area = area;}

  square*  get_link(             int  i) const noexcept{return m_link_table[i]     ;}
  void     set_link(square*  sq, int  i)       noexcept{       m_link_table[i] = sq;}

  square_data&  get_data(                 ) const noexcept{return *m_data       ;}
  void          set_data(square_data&  dat)       noexcept{        m_data = &dat;}

  template<typename  T>
  T&  get_data() const noexcept{return *static_cast<T*>(m_data);}

};


class
board
{
  int  m_width =0;
  int  m_height=0;

  int  m_square_size=0;

  int  m_image_width =0;
  int  m_image_height=0;

  std::vector<square>   m_square_table;

public:
  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  int  get_square_size() const noexcept{return m_square_size;}

  int  get_image_width()  const noexcept{return m_image_width;}
  int  get_image_height() const noexcept{return m_image_height;}

        square&  get_square(int  x, int  y)       noexcept{return m_square_table[(m_width*y)+x];}
  const square&  get_square(int  x, int  y) const noexcept{return m_square_table[(m_width*y)+x];}

        square&  get_square_by_object(const spaces::object&  o)       noexcept;
  const square&  get_square_by_object(const spaces::object&  o) const noexcept;

  void  build(int  w, int  h, int  square_size, square_data&  default_data) noexcept;

  void  put_to_around(square_data&  sqdat) noexcept;

  void  detect_collision(spaces::object&  o) noexcept;

};


class
board_view
{
  const board*  m_board=nullptr;

  const image*  m_source_image=nullptr;

  int  m_width =0;
  int  m_height=0;

  point  m_offset;

  struct rendering_context;

  void  render_line(const rendering_context&  ctx) const noexcept;

public:
  board_view(                                 ) noexcept{}
  board_view(const board&  brd, int  w, int  h) noexcept{reset(brd,w,h);}

  void  set_source_image(const image&  img) noexcept{m_source_image = &img;}

  void  chase_object(const spaces::object&  obj, int speed) noexcept;

  void  reset(const board&  brd, int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_offset(int  x, int  y) noexcept{m_offset  = point(x,y);}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}

  const point&  get_offset() const noexcept{return m_offset;}

  void  correct_offset() noexcept;

  void  render(image_cursor  cur, void  (*callback)(board_view&  bv, int  output_line)=nullptr) noexcept;

};


}


}




#endif




