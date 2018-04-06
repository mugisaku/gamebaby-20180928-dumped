#ifndef gbstd_window_hpp_was_included
#define gbstd_window_hpp_was_included


#include"libgbstd/widget.hpp"


namespace gbstd{
namespace windows{


class window_manager;


class
window
{
  friend class window_manager;

  uint32_t  m_number;

  window_manager*  m_manager;

  image  m_image;

  container  m_container;

  widget*  m_current=nullptr;

  color_index  m_colors[4] = {predefined::null      ,
                              predefined::blue      ,
                              predefined::white     ,
                              predefined::light_gray,};

  point  m_point;

  bool  m_modified=true;

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
  window(uint32_t  n, int  x=0, int  y=0) noexcept;

  container*  operator->() noexcept{return &m_container;}

  container&  get_container() noexcept{return m_container;}

  window_manager*  get_manager() const noexcept{return m_manager;}

  void          set_point(point  pt)       noexcept{       m_point = pt;}
  const point&  get_point(         ) const noexcept{return m_point     ;}

  bool  test_by_point(int  x, int  y) const noexcept;

  bool  is_image_modified() noexcept;

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

  void   react() noexcept;
  void  update() noexcept;

};


class
window_pointer
{
  friend class window_manager;

  window*  m_data=nullptr;

public:
  window_pointer() noexcept{}
  window_pointer(window&  w) noexcept: m_data(&w){}

  window*  operator->() const noexcept{return  m_data;}
  window&  operator *() const noexcept{return *m_data;}

  operator bool() const noexcept{return m_data;}

  bool  operator==(const window_pointer&  rhs) const noexcept{return m_data == rhs.m_data;}
  bool  operator!=(const window_pointer&  rhs) const noexcept{return m_data != rhs.m_data;}

};


class
window_manager
{
  window*  m_bottom=nullptr;
  window*  m_top   =nullptr;

  uint32_t  m_number_of_windows=0;

  bool  m_modified_flag=true;

  bool  m_moving_flag=false;

  point  m_gripping_point;

  void   touch(window&  win) noexcept;
  void  remove(window&  win) noexcept;

public:
  window_manager() noexcept{}
 ~window_manager(){clear();}

  void  clear()  noexcept;

  window_pointer  new_window(int  x, int  y) noexcept;

  void  delete_window(window_pointer  ptr) noexcept;

  bool  is_any_window_modified() noexcept;

  void  update() noexcept;

  void  composite(image&  dst) noexcept;

};


}

using windows::window;
using windows::window_manager;


}


#endif




