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
piece
{
  square*  m_square=nullptr;

  int  m_mv    =0;
  int  m_mv_max=1;

public:
  void     set_square(square*  sq)       noexcept{       m_square = sq;}
  square*  get_square(           ) const noexcept{return m_square     ;}

  int  get_mv() const noexcept{return m_mv;}

  void  set_mv(int  v) noexcept{m_mv  = v;}
  void  add_mv(int  v) noexcept{m_mv += v;}

  int  get_mv_max() const noexcept{return m_mv_max;}

  void  set_mv_max(int  v) noexcept{m_mv_max  = v;}
  void  add_mv_max(int  v) noexcept{m_mv_max += v;}

  void  reset_mv() noexcept{m_mv = m_mv_max;}

  virtual int  get_mv_consumption(const square&  sq) const noexcept{return 0;}

};


class
square
{
  point  m_index;

  area  m_area;

  piece*  m_piece=nullptr;

  uint32_t  m_route_code=0;

  square_data*  m_data=nullptr;

public:
  static constexpr int  size = 24;

  uint32_t  get_mv(           ) const noexcept{return m_route_code>>1;}
  void      set_mv(uint32_t  v)       noexcept{m_route_code = (v<<1)|1;}

  bool  test_mark() const noexcept{return m_route_code&1;}

  void  reset_route_code() noexcept{m_route_code = 0;}

  const point&  get_index(         ) const noexcept{return m_index     ;}
  void          set_index(point  pt)       noexcept{       m_index = pt;}

  const area&  get_area(          ) const noexcept{return m_area       ;}
  void         set_area(area  area)       noexcept{       m_area = area;}

  piece*  get_piece(         ) const noexcept{return m_piece    ;}
  void    set_piece(piece*  p)       noexcept{       m_piece = p;}

  square_data&  get_data(                 ) const noexcept{return *m_data       ;}
  void          set_data(square_data&  dat)       noexcept{        m_data = &dat;}

  template<typename  T>
  T&  get_data() const noexcept{return *static_cast<T*>(m_data);}

  point  get_image_base_point() const noexcept{return m_data? m_data->get_image_point():point();}

};


template<typename  squareT>
class
basic_board
{
  static constexpr int  m_square_size = squareT::size;

  int  m_width =0;
  int  m_height=0;

  int  m_image_width =0;
  int  m_image_height=0;

  std::vector<squareT>   m_square_table;

  std::vector<squareT*>   m_routing_stack;

public:
  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  constexpr int  get_square_size() const noexcept{return m_square_size;}

  int  get_image_width()  const noexcept{return m_image_width;}
  int  get_image_height() const noexcept{return m_image_height;}

  bool  test_index(point  i) const noexcept;

        squareT&  get_square(int  x, int  y)       noexcept{return m_square_table[(m_width*y)+x];}
  const squareT&  get_square(int  x, int  y) const noexcept{return m_square_table[(m_width*y)+x];}

        squareT&  get_square(point  i)       noexcept{return get_square(i.x,i.y);}
  const squareT&  get_square(point  i) const noexcept{return get_square(i.x,i.y);}

        squareT*  try_get_square(point  i)       noexcept{return test_index(i)? &get_square(i):nullptr;}
  const squareT*  try_get_square(point  i) const noexcept{return test_index(i)? &get_square(i):nullptr;}

        squareT&  get_square_by_object(const spaces::object&  o)       noexcept;
  const squareT&  get_square_by_object(const spaces::object&  o) const noexcept;

  void  build(int  w, int  h) noexcept;

  void  search_route(const piece&  p, point  a, point  b) noexcept;

};


#include"libgbstd/boards/basic_board.tcpp"


template<typename  squareT>
class
basic_board_view
{
  const basic_board<squareT>*  m_board=nullptr;

  const image*  m_source_image=nullptr;

  int  m_width =0;
  int  m_height=0;

  point  m_offset;

  struct rendering_context;

  void  render_line(const rendering_context&  ctx) const noexcept;

public:
  basic_board_view(                                                ) noexcept{}
  basic_board_view(const basic_board<squareT>&  brd, int  w, int  h) noexcept{reset(brd,w,h);}

  void  set_source_image(const image&  img) noexcept{m_source_image = &img;}

  void  chase_object(const spaces::object&  obj, int speed) noexcept;

  void  reset(const basic_board<squareT>&  brd, int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_offset(int  x, int  y) noexcept{m_offset  = point(x,y);}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}

  const point&  get_offset() const noexcept{return m_offset;}

  void  correct_offset() noexcept;

  void  render(image_cursor  cur, void  (*callback)(basic_board_view<squareT>&  bv, int  output_line)=nullptr) noexcept;

};


#include"libgbstd/boards/basic_board_view.tcpp"


}


using square     = boards::square;
using board      = boards::basic_board<boards::square>;
using board_view = boards::basic_board_view<boards::square>;



}




#endif




