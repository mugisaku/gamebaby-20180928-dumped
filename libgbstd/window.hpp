#ifndef gbstd_window_hpp_was_included
#define gbstd_window_hpp_was_included


#include"libgbstd/widget.hpp"
#include"libgbstd/text.hpp"


namespace gbstd{
namespace windows{


class window_manager;


struct
window_style
{
  color  colors[4] = {color()                     ,
                      predefined_color::blue      ,
                      predefined_color::white     ,
                      predefined_color::light_gray,};

};


class
window
{
protected:
  gbstd::string  m_name;

  window_manager*  m_manager=nullptr;

  image  m_frame_image;
  image  m_content_image;

  widgets::root  m_root;

  widget*  m_current=nullptr;

  int  m_width;
  int  m_height;

  point  m_point;

  uint32_t  m_state=0;

  window*  m_low =nullptr;
  window*  m_high=nullptr;

  window*  m_child=nullptr;

  struct flags{
    static constexpr uint32_t   header = 0x01;
    static constexpr uint32_t  movable = 0x02;
    static constexpr uint32_t   active = 0x04;

    static constexpr uint32_t  needed_to_update_frame = 0x08;
    static constexpr uint32_t  needed_to_redraw       = 0x10;

  };


  void    set_flag(uint32_t  v) noexcept{m_state |=  v;}
  void  unset_flag(uint32_t  v) noexcept{m_state &= ~v;}

  void  modify_flag(uint32_t  v1, uint32_t  v2) noexcept
  {
    m_state &= ~v1;
    m_state |=  v2;
  }

  bool   test_flag(uint32_t  v) const noexcept{return m_state&v;}

public:
  window(gbstd::string_view  name, widget*  widget) noexcept;

  widgets::root&  get_root() noexcept{return m_root;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void             set_manager(window_manager*  man)       noexcept{       m_manager = man;}
  window_manager*  get_manager(                    ) const noexcept{return m_manager      ;}

  void          set_point(point  pt)       noexcept{       m_point = pt;}
  const point&  get_point(         ) const noexcept{return m_point     ;}

  bool  test_by_point(int  x, int  y) const noexcept;

  bool  is_movable() const noexcept{return test_flag(flags::movable);}

  void    set_movable_flag() noexcept{  set_flag(flags::movable);}
  void  unset_movable_flag() noexcept{unset_flag(flags::movable);}

  bool  is_active() const noexcept{return test_flag(flags::active);}

  void    set_active_flag() noexcept{   set_flag(flags::active|flags::needed_to_update_frame);}
  void  unset_active_flag() noexcept{modify_flag(flags::active,flags::needed_to_update_frame);}

  bool  has_header() const noexcept{return test_flag(flags::header);}

  void    set_header_flag() noexcept{   set_flag(flags::header|flags::needed_to_update_frame);}
  void  unset_header_flag() noexcept{modify_flag(flags::header,flags::needed_to_update_frame);}

  bool  is_needed_to_redraw() const noexcept{return test_flag(flags::needed_to_redraw);}

  void  need_to_redraw() noexcept{set_flag(flags::header|flags::needed_to_redraw);}

        image&  get_frame_image()             noexcept{return m_frame_image;}
  const image&  get_const_frame_image() const noexcept{return m_frame_image;}

        image&  get_content_image()             noexcept{return m_content_image;}
  const image&  get_const_content_image() const noexcept{return m_content_image;}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  const widget*  get_current() const noexcept{return m_current;}

  void     set_high(window*  w)       noexcept{       m_high = w;}
  window*  get_high(          ) const noexcept{return m_high    ;}

  void     set_low(window*  w)       noexcept{       m_low = w;}
  window*  get_low(          ) const noexcept{return m_low;}

  window*  get_child() const noexcept{return m_child;}

  void   react(const control_device&  condev) noexcept;
  void  update() noexcept;

  void  update_frame_image() noexcept;

  void  redraw_content(image&  dst) noexcept;
  void  redraw_frame(image&  dst) noexcept;

  void  print() const noexcept;

};


class
window_manager
{
  struct flags{
    static constexpr int  needed_to_refresh = 0x01;
    static constexpr int      moving_window = 0x02;
    static constexpr int     touched_window = 0x04;

  };

  uint32_t  m_state=0;

  window*  m_bottom=nullptr;
  window*  m_top   =nullptr;

  window_style    m_active_window_style;
  window_style  m_inactive_window_style;

  uint32_t  m_time=0;

  mouse  m_old_mouse;
  mouse  m_mouse;

  point  m_gripping_point;

  window*  scan_other_windows_than_top(point  pt) noexcept;

  void    set_flag(uint32_t  flag) noexcept{m_state |=  flag;}
  void  unset_flag(uint32_t  flag) noexcept{m_state &= ~flag;}

public:
  window_manager() noexcept;
 ~window_manager(){clear();}

  void  clear()  noexcept;

  const window_style&  get_active_window_style() const noexcept{return m_active_window_style;}
  const window_style&  get_inactive_window_style() const noexcept{return m_inactive_window_style;}

  window*  append(window*  w, int  x, int  y) noexcept;
  window*  remove(window*  w) noexcept;

  uint32_t  get_time() const noexcept{return m_time;}

  const mouse&  get_mouse() const noexcept{return m_mouse;}

  bool  is_needed_to_refresh() const noexcept{return m_state&flags::needed_to_refresh;}
  bool  is_moving_window()     const noexcept{return m_state&flags::moving_window;}
  bool  is_touched_window()    const noexcept{return m_state&flags::touched_window;}

  void  need_to_refresh() noexcept{set_flag(flags::needed_to_refresh);}

  void  put_down_window_all() const noexcept;

  void  update(const control_device&  condev) noexcept;

  bool  composite(image&  dst) noexcept;

  void  print() const noexcept;

};


}

using windows::window;
using windows::window_manager;


}


#endif




