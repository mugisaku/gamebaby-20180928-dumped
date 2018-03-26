#ifndef gbstd_widget_hpp_was_included
#define gbstd_widget_hpp_was_included


#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
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


class container;


class
widget
{
   friend class container;
   friend class root;

protected:
  struct flags{
    static constexpr uint32_t  shown                   = 0x0001;
    static constexpr uint32_t  frozen                  = 0x0002;
    static constexpr uint32_t  need_to_redraw_children = 0x0004;
    static constexpr uint32_t  need_to_redraw_self     = 0x0008;
    static constexpr uint32_t  needed_to_reform        = 0x0010;
  };


  uint32_t  m_flags=0;

  container*  m_container=nullptr;

  point  m_absolute_point;
  point  m_absolute_end_point;

  point  m_relative_point;

  int  m_width =0;
  int  m_height=0;

  style  m_style;

  const style*  m_current_style=nullptr;

  widget*  m_previous=nullptr;
  widget*  m_next    =nullptr;

  void    set_flag(uint32_t  v) noexcept{m_flags |=  v;}
  void  unset_flag(uint32_t  v) noexcept{m_flags &= ~v;}
  bool   test_flag(uint32_t  v) const noexcept{return m_flags&v;}

  void  notify_flag(uint32_t  v) noexcept;

  virtual void  reform() noexcept;

public:
  widget(int  w=1, int  h=1) noexcept: m_width(w), m_height(h){}
  widget(const widget&   rhs) noexcept=delete;
  widget(      widget&&  rhs) noexcept=delete;
  virtual ~widget(){}

  widget&  operator=(const widget&   rhs) noexcept=delete;
  widget&  operator=(      widget&&  rhs) noexcept=delete;

  virtual void  do_when_cursor_got_in()             noexcept{}
  virtual void  do_when_cursor_got_out()            noexcept{}
  virtual void  do_when_mouse_acted(int  x, int  y) noexcept{}

  virtual void  render(image_cursor  cur) noexcept{}

  widget*  erase() noexcept;

  void  need_to_redraw() noexcept;
  void  need_to_reform() noexcept;

  bool  test_by_point(int  x, int  y) const noexcept;

  virtual widget*  scan_by_point(int  x, int  y) noexcept{return test_by_point(x,y)? this:nullptr;}

  const point&  get_absolute_point() const noexcept{return m_absolute_point;}
  const point&  get_relative_point() const noexcept{return m_relative_point;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void    freeze() noexcept{  set_flag(flags::frozen);}
  void  unfreeze() noexcept{unset_flag(flags::frozen);}

  void  show() noexcept{  set_flag(flags::shown);}
  void  hide() noexcept{unset_flag(flags::shown);}

  virtual void  show_all() noexcept{show();}

  bool  is_shown() const noexcept{return test_flag(flags::shown);}
  bool  is_frozen() const noexcept{return test_flag(flags::frozen);}

  widget*  get_previous() const noexcept{return m_previous;}
  widget*  get_next() const noexcept{return m_next;}

  void  print() const noexcept;

};




class
container: public widget
{
  friend class root;

  widget*  m_first_child=nullptr;
  widget*  m_last_child =nullptr;

  void  reform() noexcept override;

public:
  container() noexcept{}
 ~container(){clear();}

  void  clear() noexcept;

  void  render(image_cursor  cur) noexcept override;

  widget*  scan_by_point(int  x, int  y) noexcept override;

  void  append_child(widget*  child, int  x, int  y) noexcept;
  void  remove_child(widget*  child                ) noexcept;

  void  show_all() noexcept override;

};




class
root: public container
{
  widget*  m_current=nullptr;

  controller  m_previous_ctrl;

public:
  root() noexcept{}
 ~root(){}

  void  react() noexcept;

};


}}


#endif




