#ifndef gbact_board_HPP
#define gbact_board_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/position.hpp"
#include"libgbstd/area.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/board.hpp"


using namespace gbstd;


namespace gbact{


constexpr int  g_board_width  = 12;
constexpr int  g_board_height = 10;


class square;
class piece;




extern images::image
g_bg_image;




class
square_data
{
  enum class kind{
    null,
    block,
    ladder,

  } m_kind=kind::null;


  point  m_image_point;

  gate  m_gate;

  constexpr square_data(kind  k, int  x, int  y, gate  g) noexcept: m_kind(k), m_image_point(x,y), m_gate(g){}

public:
  constexpr square_data() noexcept{}

  constexpr bool  is_null()   const noexcept{return m_kind == kind::null;}
  constexpr bool  is_block()  const noexcept{return m_kind == kind::block;}
  constexpr bool  is_ladder() const noexcept{return m_kind == kind::ladder;}

  void  be_null()   noexcept{m_kind = kind::null;}
  void  be_block()  noexcept{m_kind = kind::block;}
  void  be_ladder() noexcept{m_kind = kind::ladder;}

  static constexpr square_data     null(){return square_data(kind::null  ,24*0,24*0, gate::get_none());}
  static constexpr square_data   block0(){return square_data(kind::block ,24*0,24*1, gate::get_all());}
  static constexpr square_data   block1(){return square_data(kind::block ,24*2,24*0, gate::get_all());}
  static constexpr square_data  ladder0(){return square_data(kind::ladder,24*1,24*0, gate::get_top());}
  static constexpr square_data  ladder1(){return square_data(kind::ladder,24*1,24*1, gate::get_none());}


  constexpr point  get_image_point(         ) const noexcept{return m_image_point     ;}
  void             set_image_point(point  pt)       noexcept{       m_image_point = pt;}

  raster_view  get_raster_view() const noexcept
  {
    auto&  pix = g_bg_image.get_pixel(m_image_point.x,m_image_point.y);

    return raster_view(&pix,g_bg_image.get_width());
  }

  constexpr gate  get_gate(       ) const noexcept{return m_gate    ;}
  void            set_gate(gate  g)       noexcept{       m_gate = g;}

};


class
square
{
  point  m_index;

  area  m_area;

  square_data*  m_data=nullptr;

  static const pixel  m_dummy_line[24*24];

public:
  static constexpr int  size = 24;

  operator raster_view() const noexcept{return m_data? m_data->get_raster_view():raster_view(m_dummy_line,size);}

  const point&  get_index(         ) const noexcept{return m_index     ;}
  void          set_index(point  pt)       noexcept{       m_index = pt;}

  const area&  get_area(          ) const noexcept{return m_area       ;}
  void         set_area(area  area)       noexcept{       m_area = area;}

  square_data&  get_data(                 ) const noexcept{return *m_data       ;}
  void          set_data(square_data&  dat)       noexcept{        m_data = &dat;}

  template<typename  T>
  T&  get_data() const noexcept{return *static_cast<T*>(m_data);}

};




void  reset_board() noexcept;


extern basic_board<square,g_board_width,g_board_height>
g_board;


extern basic_board_view<square,g_board_width,g_board_height>
g_board_view;





}




#endif




