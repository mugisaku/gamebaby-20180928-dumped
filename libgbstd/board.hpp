#ifndef board_HPP
#define board_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/position.hpp"
#include"libgbstd/area.hpp"
#include"libgbstd/space.hpp"


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




template<typename  squareT, int  W, int  H>
class
basic_board
{
  static constexpr int  m_square_size = squareT::size;

  static constexpr int  m_width =W;
  static constexpr int  m_height=H;

  static constexpr int  m_image_width =m_square_size*W;
  static constexpr int  m_image_height=m_square_size*H;

  squareT  m_square_table[W*H];

  std::vector<squareT*>   m_routing_stack;

public:
  constexpr int  get_width()  const noexcept{return m_width;}
  constexpr int  get_height() const noexcept{return m_height;}

  constexpr int  get_square_size() const noexcept{return m_square_size;}

  constexpr int  get_image_width()  const noexcept{return m_image_width;}
  constexpr int  get_image_height() const noexcept{return m_image_height;}

  bool  test_index(point  i) const noexcept;

        squareT&  get_square(int  x, int  y)       noexcept{return m_square_table[(m_width*y)+x];}
  const squareT&  get_square(int  x, int  y) const noexcept{return m_square_table[(m_width*y)+x];}

        squareT&  get_square(point  i)       noexcept{return get_square(i.x,i.y);}
  const squareT&  get_square(point  i) const noexcept{return get_square(i.x,i.y);}

        squareT*  try_get_square(point  i)       noexcept{return test_index(i)? &get_square(i):nullptr;}
  const squareT*  try_get_square(point  i) const noexcept{return test_index(i)? &get_square(i):nullptr;}

        squareT&  get_square_by_object(const spaces::object&  o)       noexcept;
  const squareT&  get_square_by_object(const spaces::object&  o) const noexcept;

};


#include"libgbstd/boards/basic_board.tcpp"


template<typename  squareT, int  W, int  H>
class
basic_board_view
{
  const basic_board<squareT,W,H>*  m_board=nullptr;

  const image*  m_source_image=nullptr;

  int  m_width =0;
  int  m_height=0;

  point  m_offset;

  bool  m_visible=true;

  struct rendering_context;

  void  render_line(const rendering_context&  ctx) const noexcept;

public:
  basic_board_view(                                                    ) noexcept{}
  basic_board_view(const basic_board<squareT,W,H>&  brd, int  w, int  h) noexcept{reset(brd,w,h);}

  bool  is_visible() const noexcept{return m_visible;}

  void  show() noexcept{m_visible =  true;}
  void  hide() noexcept{m_visible = false;}

  void  set_source_image(const image&  img) noexcept{m_source_image = &img;}

  void  chase_object(const spaces::object&  obj, int speed) noexcept;

  void  reset(const basic_board<squareT,W,H>&  brd, int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_offset(int  x, int  y) noexcept{m_offset  = point(x,y);}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}

  const point&  get_offset() const noexcept{return m_offset;}

  void  correct_offset() noexcept;

  void  render(image_cursor  cur, void  (*callback)(basic_board_view<squareT,W,H>&  bv, int  output_line)=nullptr) noexcept;

};


#include"libgbstd/boards/basic_board_view.tcpp"



}




#endif




