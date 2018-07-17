#ifndef gbwar_board_HPP
#define gbwar_board_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/position.hpp"
#include"libgbstd/area.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/text.hpp"


using namespace gbstd;


constexpr int  g_board_width  = 12;
constexpr int  g_board_height = 10;


class square;
class piece;


class
square_data
{
  enum class kind{} m_kind;

  point  m_image_point;

  gate  m_gate;

  constexpr square_data(kind  k, int  x, int  y, gate  g) noexcept: m_kind(k), m_image_point(x,y), m_gate(g){}

public:
  square_data() noexcept{}

  constexpr point  get_image_point(         ) const noexcept{return m_image_point     ;}
  void             set_image_point(point  pt)       noexcept{       m_image_point = pt;}

  constexpr gate  get_gate(       ) const noexcept{return m_gate    ;}
  void            set_gate(gate  g)       noexcept{       m_gate = g;}

};


class
square
{
  point  m_index;

  piece*  m_piece=nullptr;

  uint32_t  m_distance=0;

  uint32_t  m_total_mv_consumption=0;

  bool  m_lighting=false;

  square_data*  m_data=nullptr;

public:
  static constexpr int  size = 24;

  void    lighten() noexcept{m_lighting =  true;}
  void  unlighten() noexcept{m_lighting = false;}

  bool  is_lightened() const noexcept{return m_lighting;}

  void       set_total_mv_consumption(uint32_t  v)       noexcept{       m_total_mv_consumption = v;}
  uint32_t   get_total_mv_consumption(           ) const noexcept{return m_total_mv_consumption    ;}

  void      set_distance(uint32_t  v)       noexcept{       m_distance = v;}
  uint32_t  get_distance(           ) const noexcept{return m_distance    ;}

  const point&  get_index(         ) const noexcept{return m_index     ;}
  void          set_index(point  pt)       noexcept{       m_index = pt;}

  square_data&  get_data(                 ) const noexcept{return *m_data       ;}
  void          set_data(square_data&  dat)       noexcept{        m_data = &dat;}

  piece*  get_piece(         ) const noexcept{return m_piece    ;}
  void    set_piece(piece*  p)       noexcept;

  template<typename  T>
  T&  get_data() const noexcept{return *static_cast<T*>(m_data);}

  point  get_image_base_point() const noexcept;

  void  print() const noexcept;

};


class
square_info: public spaces::object
{
  square*  m_target=nullptr;

public:
  square_info() noexcept{}

  void     set_target(square*  sq)       noexcept{       m_target = sq;}
  square*  get_target(           ) const noexcept{return m_target     ;}

  void  update_core() noexcept override;

  void  render(point  offset, image_cursor  cur) noexcept override;

};


enum class
instruction
{
  nop,

  movu,
  movul,
  movur,

  movl,
  movr,

  movd,
  movdl,
  movdr,

};


struct
route
{
  std::vector<instruction>  codes;

};


route  search_route(const square&  start, const square&  goal) noexcept;
route  search_route(point  start, point  goal) noexcept;

void  reset_board() noexcept;
void  clean_board() noexcept;


extern basic_board<square,g_board_width,g_board_height>
g_board;


extern basic_board_view<square,g_board_width,g_board_height>
g_board_view;




#endif




