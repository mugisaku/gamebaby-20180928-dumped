#ifndef gbstd_widget_hpp_was_included
#define gbstd_widget_hpp_was_included


#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"libgbstd/string.hpp"
#include"libgbstd/utility.hpp"
#include<initializer_list>
#include<memory>


namespace gbstd{
namespace widgets{


class widget;
class container;


class
widget
{
protected:
  gbstd::string  m_name;

  uint32_t  m_flags=0;

  widget*  m_parent=nullptr;

  point  m_absolute_point;
  point  m_relative_point;

  int  m_width =0;
  int  m_height=0;

  void*  m_userdata=nullptr;

  void  (*m_deleter)(void*  ptr)=nullptr;

public:
  struct flags{
    static constexpr uint32_t  shown                   = 0x0001;
    static constexpr uint32_t  frozen                  = 0x0002;
    static constexpr uint32_t  needed_to_redraw        = 0x0004;
    static constexpr uint32_t  needed_to_reform        = 0x0008;
  };


  widget(int  w=1, int  h=1) noexcept: m_width(w), m_height(h){need_to_reform();}
  widget(const widget&   rhs) noexcept=delete;
  widget(      widget&&  rhs) noexcept=delete;
  virtual ~widget(){clear();}

  widget&  operator=(const widget&   rhs) noexcept=delete;
  widget&  operator=(      widget&&  rhs) noexcept=delete;

  virtual const char*  get_widget_name() const noexcept{return "widget";}

  virtual void  reform(point  base_pt) noexcept;
  virtual void  redraw(image&  img) noexcept;

  virtual void  do_when_cursor_got_in()             noexcept{}
  virtual void  do_when_cursor_got_out()            noexcept{}
  virtual void  do_when_mouse_acted(int  x, int  y) noexcept{}

  virtual void  render(image_cursor  cur) noexcept{}

  virtual bool  remove(widget*  target) noexcept{return false;}

  virtual void  clear() noexcept;

  void  need_to_redraw() noexcept;
  void  need_to_reform() noexcept;


  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  const gbstd::string&  get_name(                        ) const noexcept{return m_name       ;}


  void     set_parent(widget*  parent)       noexcept{       m_parent = parent;}
  widget*  get_parent(               ) const noexcept{return m_parent         ;}

  void*  get_userdata() const noexcept{return m_userdata;}

  template<typename  T>  void  set_userdata(T*  ptr,  void  (*deleter)(T*  ptr)=nullptr) noexcept
  {
    m_userdata = ptr;
    m_deleter  = reinterpret_cast<void(*)(void*)>(deleter);
  }


  bool  test_by_point(int  x, int  y) const noexcept;

  virtual widget*  scan_by_point(int  x, int  y) noexcept{return test_by_point(x,y)? this:nullptr;}

  const point&  get_absolute_point() const noexcept{return m_absolute_point;}
  const point&  get_relative_point() const noexcept{return m_relative_point;}

  void  set_relative_point(point  pt) noexcept{m_relative_point = pt;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}


  void    set_flag(uint32_t  v) noexcept{m_flags |=  v;}
  void  unset_flag(uint32_t  v) noexcept{m_flags &= ~v;}
  bool   test_flag(uint32_t  v) const noexcept{return m_flags&v;}

  void  notify_flag(uint32_t  v) noexcept;


  void    freeze() noexcept{  set_flag(flags::frozen);}
  void  unfreeze() noexcept{unset_flag(flags::frozen);}

  void  show() noexcept{  set_flag(flags::shown);}
  void  hide() noexcept{unset_flag(flags::shown);}

  void  reform_if_needed(point  base_pt) noexcept;
  void  redraw_if_needed(image&  img) noexcept;

  virtual void  show_all() noexcept{show();}

  bool  is_shown() const noexcept{return test_flag(flags::shown);}
  bool  is_frozen() const noexcept{return test_flag(flags::frozen);}

  bool  is_needed_to_reform() const noexcept{return test_flag(flags::needed_to_reform);}
  bool  is_needed_to_redraw() const noexcept{return test_flag(flags::needed_to_redraw);}

  virtual void  print(int  indent=0) const noexcept;

};




class
container: public widget
{
protected:
  std::vector<std::unique_ptr<widget>>  m_children;

public:
  const char*  get_widget_name() const noexcept override{return "container";}

  void  clear() noexcept override;
  bool  remove(widget*  target) noexcept override;

  void  reform(point  base_pt) noexcept override;
  void  render(image_cursor  cur) noexcept override;

  widget*  scan_by_point(int  x, int  y) noexcept override;

  void  append_child(widget*  child, int  x, int  y) noexcept;

  void  show_all() noexcept override;

  void  print(int  indent=0) const noexcept override;

};




class window_manager;


class
window
{
  friend class window_manager;

  window_manager*  m_manager;

  container  m_container;

  widget*  m_current=nullptr;

  image  m_image;

  pixel  m_pixels[4] = {pixel(predefined::null),
                        pixel(predefined::blue      ,30000),
                        pixel(predefined::white     ,30000),
                        pixel(predefined::light_gray,30000)};

  point  m_point;

  bool  m_modified_flag=true;

  window*  m_low =nullptr;
  window*  m_high=nullptr;

  void  draw_frame() noexcept;

public:
  window(int  x=0, int  y=0) noexcept;

  container*  operator->() noexcept{return &m_container;}

  container&  get_container() noexcept{return m_container;}

  const point&  get_point() const noexcept{return m_point;}

  bool  test_by_point(int  x, int  y) const noexcept;

  bool  is_image_modified() noexcept;

  const image&  get_image() const noexcept{return m_image;}

  int  get_width()  const noexcept{return m_image.get_width() ;}
  int  get_height() const noexcept{return m_image.get_height();}

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

  bool  m_modified_flag=true;

  void   touch(window&  win) noexcept;
  void  remove(window&  win) noexcept;

public:
  window_manager() noexcept{}
 ~window_manager(){clear();}

  void  clear()  noexcept;

  window_pointer  new_window(int  x, int  y) noexcept;

  void  delete_window(window_pointer  ptr) noexcept;

  void  reset_windows_all() noexcept;

  bool  is_any_window_modified() noexcept;

  void  update() noexcept;

  void  composite(image&  dst) noexcept;

};




class
label: public widget
{
  gbstd::u16string  m_text;

public:
  label(gbstd::string_view     sv) noexcept;
  label(gbstd::u16string_view  sv) noexcept;

  const char*  get_widget_name() const noexcept override{return "label";}

  void  set_text(gbstd::u16string_view  sv) noexcept;
  void  set_text(gbstd::string_view     sv) noexcept;

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


class
icon
{
public:
  static constexpr int  size = 16;

private:
  color_index  m_data[size][size]={0};

public:
  icon(std::initializer_list<color_index>  ls) noexcept
  {
    auto   it = &m_data[   0][0];
    auto  end = &m_data[size][0];

      for(auto  i: ls)
      {
        *it++ = i;
      }
  }

  void         set_color_index(int  x, int  y, color_index  i)       noexcept{       m_data[y][x] = i;}
  color_index  get_color_index(int  x, int  y                ) const noexcept{return m_data[y][x]    ;}

};


class
icon_selector: public widget
{
  const icon*  m_icons;

  int  m_current=0;

public:
  icon_selector(const icon*  icons) noexcept: widget(icon::size,icon::size), m_icons(icons){}

  const icon&  operator*() const noexcept{return m_icons[m_current];}

  void  set_current(int  i) noexcept
  {
    m_current = i;

    need_to_redraw();
  }

  const char*  get_widget_name() const noexcept override{return "icon_selector";}

  void  render(image_cursor  cur) noexcept override;

};


class
button: public widget
{
  std::unique_ptr<widget>  m_target;

  void  (*m_callback)(button&  button)=nullptr;

  enum class state{
    released,
     pressed,
  } m_state=state::released;


  int  m_count=0;

public:
  button(widget*  target, void  (*callback)(button&)) noexcept;

  const char*  get_widget_name() const noexcept override{return "button";}

  int  get_count() const noexcept{return m_count;}

  void  reset_count() noexcept{m_count = 0;}

  bool  is_pressed()  const noexcept{return m_state ==  state::pressed;}
  bool  is_released() const noexcept{return m_state == state::released;}

  void  reform(point  base_pt) noexcept override;

  void  do_when_cursor_got_out()            noexcept override;
  void  do_when_mouse_acted(int  x, int  y) noexcept override;

  void  set_text(gbstd::u16string_view  sv) noexcept;

  void  render(image_cursor  cur) noexcept override;

  void  print(int  indent=0) const noexcept override;

  void  show_all() noexcept override;

};




class
table_column: public container
{
public:
  table_column(std::initializer_list<widget*>  ls={}) noexcept{append(ls);}

  const char*  get_widget_name() const noexcept override{return "table_column";}

  void  reform(point  base_pt) noexcept override;

  void  append(std::initializer_list<widget*>  ls) noexcept;

};


class
table_row: public container
{
public:
  table_row(std::initializer_list<widget*>  ls={}) noexcept{append(ls);}

  const char*  get_widget_name() const noexcept override{return "table_row";}

  void  reform(point  base_pt) noexcept override;

  void  append(std::initializer_list<widget*>  ls) noexcept;

};


class
dial: public table_row
{
  int  m_current=0;

  int  m_min=0;
  int  m_max=0;

  void  (*m_callback)(dial&  dial, int  old_value, int  new_value)=nullptr;

  icon_selector*     m_up_selector;
  icon_selector*   m_down_selector;

  button*     m_up_button;
  button*   m_down_button;

  label*   m_label;

  static void    up(button&  btn);
  static void  down(button&  btn);

public:
  dial(int  min, int  max, void  (*callback)(dial&,int,int)) noexcept;

  int  get_current() const noexcept{return m_current;}
  int  get_min() const noexcept{return m_min;}
  int  get_max() const noexcept{return m_max;}

};




}


using widgets::widget;


}


#endif




