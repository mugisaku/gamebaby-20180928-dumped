#ifndef gbstd_window_hpp_was_included
#define gbstd_window_hpp_was_included


#include"libgbstd/widget.hpp"
#include"libgbstd/text.hpp"


namespace gbstd{
namespace windows{


class window_manager;


class
window
{
protected:
  window_manager*  m_manager=nullptr;

  image  m_image;

  container  m_container;

  widget*  m_current=nullptr;

  color_index  m_colors[4] = {predefined::null      ,
                              predefined::blue      ,
                              predefined::white     ,
                              predefined::light_gray,};

  point  m_point;

  uint32_t  m_state=0;

  window*  m_low =nullptr;
  window*  m_high=nullptr;

  void  draw_frame() noexcept;

  struct flags{
    static constexpr uint32_t  transparent = 0x01;
    static constexpr uint32_t       header = 0x02;

  };

  void  change_state(uint32_t  st) noexcept;

public:
  window() noexcept;
  virtual ~window() noexcept{}

  container*  operator->() noexcept{return &m_container;}

  container&  get_container() noexcept{return m_container;}

  void             set_manager(window_manager*  man)       noexcept{       m_manager = man;}
  window_manager*  get_manager(                    ) const noexcept{return m_manager      ;}

  void          set_point(point  pt)       noexcept{       m_point = pt;}
  const point&  get_point(         ) const noexcept{return m_point     ;}

  bool  test_by_point(int  x, int  y) const noexcept;

  bool  is_transparent() const noexcept{return m_state&flags::transparent;}

  void    set_transparent_flag() noexcept{change_state(m_state| flags::transparent);}
  void  unset_transparent_flag() noexcept{change_state(m_state&~flags::transparent);}

  void    set_header_flag() noexcept{change_state(m_state| flags::header);}
  void  unset_header_flag() noexcept{change_state(m_state&~flags::header);}

        image&  get_image()             noexcept{return m_image;}
  const image&  get_const_image() const noexcept{return m_image;}

  int  get_width()  const noexcept{return get_const_image().get_width() ;}
  int  get_height() const noexcept{return get_const_image().get_height();}

  const widget*  get_current() const noexcept{return m_current;}

  void     set_high(window*  w)       noexcept{       m_high = w;}
  window*  get_high(          ) const noexcept{return m_high    ;}

  void     set_low(window*  w)       noexcept{       m_low = w;}
  window*  get_low(          ) const noexcept{return m_low;}

  void   react() noexcept;
  bool  update() noexcept;

};


class
window_manager
{
  window*  m_bottom=nullptr;
  window*  m_top   =nullptr;

  bool  m_needing_to_refresh=true;

  bool  m_moving_flag=false;

  point  m_gripping_point;

  void   touch(window&  win) noexcept;

public:
  window_manager() noexcept{}
 ~window_manager(){clear();}

  void  clear()  noexcept;

  window*  append(window*  w, int  x, int  y) noexcept;
  window*  remove(window*  w) noexcept;

  void  update() noexcept;

  bool  composite(image&  dst) noexcept;

};


}

using windows::window;
using windows::window_manager;


}


#endif




