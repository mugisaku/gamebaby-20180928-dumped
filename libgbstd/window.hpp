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

  image  m_image;

  widgets::root  m_root;

  widget*  m_current=nullptr;

  point  m_point;

  uint32_t  m_state=0;

  window*  m_low =nullptr;
  window*  m_high=nullptr;

  window*  m_child=nullptr;

  void  draw_frame(const window_style&  style, image_cursor  cur) noexcept;

  struct flags{
    static constexpr uint32_t       header = 0x01;
    static constexpr uint32_t      movable = 0x02;
    static constexpr uint32_t       active = 0x04;

    static constexpr uint32_t  needed_to_redraw_frame   = 0x10;
    static constexpr uint32_t  needed_to_redraw_content = 0x20;

  };


  void    set_flag(uint32_t  v) noexcept{m_state |=  v;}
  void  unset_flag(uint32_t  v) noexcept{m_state &= ~v;}
  bool   test_flag(uint32_t  v) const noexcept{return m_state&v;}

  void  change_state(uint32_t  st) noexcept;

public:
  window(gbstd::string_view  name="") noexcept;

  widgets::container*  operator->() noexcept{return &m_root.get_container();}

  widgets::root&  get_root() noexcept{return m_root;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void             set_manager(window_manager*  man)       noexcept{       m_manager = man;}
  window_manager*  get_manager(                    ) const noexcept{return m_manager      ;}

  void          set_point(point  pt)       noexcept{       m_point = pt;}
  const point&  get_point(         ) const noexcept{return m_point     ;}

  bool  test_by_point(int  x, int  y) const noexcept;

  bool  is_movable() const noexcept{return m_state&flags::movable;}

  void    set_movable_flag() noexcept{change_state(m_state| flags::movable);}
  void  unset_movable_flag() noexcept{change_state(m_state&~flags::movable);}

  bool  is_active() const noexcept{return m_state&flags::active;}

  void    set_active_flag() noexcept{change_state(m_state| flags::active);}
  void  unset_active_flag() noexcept{change_state(m_state&~flags::active);}

  void    set_header_flag() noexcept{change_state(m_state| flags::header);}
  void  unset_header_flag() noexcept{change_state(m_state&~flags::header);}

  void  need_to_redraw_frame()   noexcept{m_state |= flags::needed_to_redraw_frame;}
  void  need_to_redraw_content() noexcept{m_state |= flags::needed_to_redraw_content;}

        image&  get_image()             noexcept{return m_image;}
  const image&  get_const_image() const noexcept{return m_image;}

  int  get_width()  const noexcept{return get_const_image().get_width() ;}
  int  get_height() const noexcept{return get_const_image().get_height();}

  const widget*  get_current() const noexcept{return m_current;}

  void     set_high(window*  w)       noexcept{       m_high = w;}
  window*  get_high(          ) const noexcept{return m_high    ;}

  void     set_low(window*  w)       noexcept{       m_low = w;}
  window*  get_low(          ) const noexcept{return m_low;}

  window*  get_child() const noexcept{return m_child;}

  void   react() noexcept;

  bool  update() noexcept;

  void  redraw(const window_style&  style, image&  dst) noexcept;

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

  window_style  m_active_window_style={color()                     ,
                                       predefined_color::blue      ,
                                       predefined_color::white     ,
                                       predefined_color::light_gray};

  window_style  m_inactive_window_style={color()                     ,
                                         color(0,0,5)      ,
                                         predefined_color::light_gray     ,
                                         predefined_color::dark_gray};

  point  m_gripping_point;

  window*  scan_other_windows_than_top(point  pt) noexcept;

  void    set_flag(uint32_t  flag) noexcept{m_state |=  flag;}
  void  unset_flag(uint32_t  flag) noexcept{m_state &= ~flag;}

public:
  window_manager() noexcept{}
 ~window_manager(){clear();}

  void  clear()  noexcept;

  window*  append(window*  w, int  x, int  y) noexcept;
  window*  remove(window*  w) noexcept;

  bool  is_needed_to_refresh() const noexcept{return m_state&flags::needed_to_refresh;}
  bool  is_moving_window()     const noexcept{return m_state&flags::moving_window;}
  bool  is_touched_window()    const noexcept{return m_state&flags::touched_window;}

  void  need_to_refresh() noexcept{set_flag(flags::needed_to_refresh);}

  void  update() noexcept;

  bool  composite(image&  dst) noexcept;

  void  print() const noexcept;

};


}

using windows::window;
using windows::window_manager;


}


#endif




