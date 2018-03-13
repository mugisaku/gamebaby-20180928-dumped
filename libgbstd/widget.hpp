#ifndef gbstd_widget_hpp_was_included
#define gbstd_widget_hpp_was_included


#include"libgbstd/image.hpp"
#include<memory>


namespace gbstd{
namespace widgets{


class widget;


class
style
{
  color_index  m_area_color_index;

  color_index  m_text_color_index[4];

};


class
mouse
{
  enum class kind{
    enter,
    leave,
    move,
    button_up,
    button_down,

  } m_kind;


  int  m_button_state=0;

  point  m_point;

public:
  const point*  operator->() const noexcept{return &m_point;}

  int  operator&(int  mask) const noexcept{return m_button_state&mask;}

};


class
widget
{
protected:
  struct flags{
    static constexpr uint32_t  shown                   = 0x0001;
    static constexpr uint32_t  frozen                  = 0x0002;
    static constexpr uint32_t  need_to_redraw_children = 0x0004;
    static constexpr uint32_t  need_to_redraw_self     = 0x0008;
  };


  uint32_t  m_flags=0;

  widget*  m_parent=nullptr;

  point  m_relative_point;

  image_frame  m_frame;

  style  m_style;

  const style*  m_current_style=nullptr;

  widget*  m_first_child=nullptr;
  widget*  m_last_child =nullptr;

  widget*  m_previous_sibling=nullptr;
  widget*  m_next_sibling    =nullptr;

  void    set_flag(uint32_t  v) noexcept{m_flags |=  v;}
  void  unset_flag(uint32_t  v) noexcept{m_flags &= ~v;}
  bool   test_flag(uint32_t  v) const noexcept{return m_flags&v;}

  void  remove_child(widget*  child) noexcept;

  void  notify_flag(uint32_t  v) noexcept;

public:
  widget(image&  img, int  x, int  y) noexcept: m_frame(img), m_relative_point(x,y){}
  widget(const widget&   rhs) noexcept=delete;
  widget(      widget&&  rhs) noexcept=delete;
  virtual ~widget();

  widget&  operator=(const widget&   rhs) noexcept=delete;
  widget&  operator=(      widget&&  rhs) noexcept=delete;

  virtual void  process(const mouse&  mouse) noexcept{}

  virtual void  render() noexcept;

  void  append_child(widget*  child, int  x, int  y) noexcept;

  void  erase() noexcept;

  void  redraw() noexcept;

  void  need_to_redraw() noexcept;

  void    freeze() noexcept{  set_flag(flags::frozen);}
  void  unfreeze() noexcept{unset_flag(flags::frozen);}

  void  show() noexcept{  set_flag(flags::shown);}
  void  hide() noexcept{unset_flag(flags::shown);}

  bool  is_shown() const noexcept{return test_flag(flags::shown);}
  bool  is_frozen() const noexcept{return test_flag(flags::frozen);}

};


}}


#endif




