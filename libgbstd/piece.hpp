#ifndef gbstd_piece_hpp_was_included
#define gbstd_piece_hpp_was_included


#include"libgbstd/widget.hpp"
#include"libgbstd/text.hpp"


namespace gbstd{
namespace pieces{


class piece_manager;


class
piece
{
  piece_manager*  m_manager=nullptr;

  const image*  m_image=nullptr;

  point  m_src_point;
  point  m_dst_point;

  int  m_width =0;
  int  m_height=0;

  uint32_t  m_state=0;

  void*  m_userdata=nullptr;

  piece*  m_previous=nullptr;
  piece*  m_next    =nullptr;

  struct flags{
    static constexpr uint32_t  transparent = 0x01;
    static constexpr uint32_t       header = 0x02;
    static constexpr uint32_t      movable = 0x04;

  };

  void  change_state(uint32_t  st) noexcept;

public:
  piece(const image&  img) noexcept;
  piece(const piece&   rhs) noexcept=delete;
  piece(      piece&&  rhs) noexcept=delete;

  piece&  operator=(const piece&   rhs) noexcept=delete;
  piece&  operator=(      piece&&  rhs) noexcept=delete;

  void            set_manager(piece_manager*  man)       noexcept{       m_manager = man;}
  piece_manager*  get_manager(                   ) const noexcept{return m_manager      ;}

  void          set_src_point(point  pt)       noexcept{       m_src_point = pt;}
  const point&  get_src_point(         ) const noexcept{return m_src_point     ;}

  void          set_dst_point(point  pt)       noexcept{       m_dst_point = pt;}
  const point&  get_dst_point(         ) const noexcept{return m_dst_point     ;}

  void  add_dst_point(point  pt) noexcept{m_dst_point += pt;}

  void  set_width(int  v)       noexcept{       m_width = v;}
  int   get_width(      ) const noexcept{return m_width    ;}

  void  set_height(int  v)       noexcept{       m_height = v;}
  int   get_height(      ) const noexcept{return m_height    ;}

  bool  test_by_point(int  x, int  y) const noexcept;

  void    set_previous(piece*  p)       noexcept{       m_previous = p;}
  piece*  get_previous(         ) const noexcept{return m_previous    ;}

  void    set_next(piece*  p)       noexcept{       m_next = p;}
  piece*  get_next(         ) const noexcept{return m_next;}

  void   react() noexcept;

  void  render(image&  dst) noexcept;

};


class
piece_manager
{
  piece*  m_first=nullptr;
  piece*  m_last =nullptr;

  bool  m_needing_to_refresh=true;

public:
  piece_manager() noexcept{}
 ~piece_manager(){clear();}

  void  clear()  noexcept;

  void  append(piece*  p, point  pt) noexcept;
  void  remove(piece*  p) noexcept;

  bool  update() noexcept;

  void  composite(image&  dst) noexcept;

};


}

using pieces::piece;
using pieces::piece_manager;


}


#endif




