#ifndef board_HPP
#define board_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/position.hpp"


namespace gbstd{


namespace spaces{
class object;
}


namespace boards{




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

  void  print() const noexcept;

};




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
piece
{
  uint32_t  m_kind_code=0;

  square*  m_square=nullptr;

public:
  uint32_t  get_kind_code(              ) const noexcept{return m_kind_code       ;}
  void      set_kind_code(uint32_t  code)       noexcept{       m_kind_code = code;}

  virtual void  render(image&  dst) noexcept{}

};


class
square_data
{
  uint32_t  m_kind_code=0;

  point  m_image_point;

public:
  uint32_t  get_kind_code(              ) const noexcept{return m_kind_code       ;}
  void      set_kind_code(uint32_t  code)       noexcept{       m_kind_code = code;}

  point  get_image_point(         ) const noexcept{return m_image_point     ;}
  void   set_image_point(point  pt)       noexcept{       m_image_point = pt;}

};


class
square
{
  point  m_index;

  area  m_area;

  square*  m_link_table[8];

  square_data*  m_data=nullptr;

  piece*  m_piece=nullptr;

public:
  const point&  get_index(         ) const noexcept{return m_index     ;}
  void          set_index(point  pt)       noexcept{       m_index = pt;}

  const area&  get_area(          ) const noexcept{return m_area       ;}
  void         set_area(area  area)       noexcept{       m_area = area;}

  square*  get_link(             int  i) const noexcept{return m_link_table[i]     ;}
  void     set_link(square*  sq, int  i)       noexcept{       m_link_table[i] = sq;}

  square_data*  get_data(                 ) const noexcept{return m_data      ;}
  void          set_data(square_data*  dat)       noexcept{       m_data = dat;}

  piece*  get_piece(         ) const noexcept{return m_piece    ;}
  void    set_piece(piece*  p)       noexcept{       m_piece = p;}

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
  int  get_corrected_x(int  x) const noexcept;
  int  get_corrected_y(int  y) const noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  int  get_square_size() const noexcept{return m_square_size;}

  int  get_image_width()  const noexcept{return m_image_width;}
  int  get_image_height() const noexcept{return m_image_height;}

        square&  get_square(int  x, int  y)       noexcept{return m_square_table[(m_width*y)+x];}
  const square&  get_square(int  x, int  y) const noexcept{return m_square_table[(m_width*y)+x];}

  void  build(int  w, int  h, int  square_size) noexcept;

  void  detect_collision(spaces::object&  o) noexcept;

};


class
board_view
{
  const board*  m_board=nullptr;

  const image*  m_source_image=nullptr;

  image  m_image;

  int  m_width =0;
  int  m_height=0;

  point  m_offset;

  point  m_base_square_point;

  point  get_base_point() const noexcept;

public:
  board_view(                                 ) noexcept{}
  board_view(const board&  brd, int  w, int  h) noexcept{reset(brd,w,h);}

  void  set_source_image(const image&  img) noexcept{m_source_image = &img;}

  void  reset(const board&  brd, int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_offset(int  x, int  y) noexcept{m_offset  = point(x,y);}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}

  const point&  get_offset() const noexcept{return m_offset;}

  void  update_image(point  base_square_point) noexcept;

  void  render(image&  dst) noexcept;

};


}


}




#endif




