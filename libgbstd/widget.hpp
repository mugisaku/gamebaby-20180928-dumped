#ifndef gbstd_widget_hpp_was_included
#define gbstd_widget_hpp_was_included


#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"libgbstd/string.hpp"
#include"libgbstd/text.hpp"
#include"libgbstd/utility.hpp"
#include<initializer_list>
#include<memory>


namespace gbstd{


namespace windows{
class window;
}


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
  virtual ~widget(){}

  widget&  operator=(const widget&   rhs) noexcept=delete;
  widget&  operator=(      widget&&  rhs) noexcept=delete;

  virtual const char*  get_widget_name() const noexcept{return "widget";}

  virtual windows::window*  get_window() const noexcept;

  virtual void  reform(point  base_pt) noexcept;
  virtual void  redraw(image&  img) noexcept;

  virtual void  do_when_cursor_got_in()             noexcept{}
  virtual void  do_when_cursor_got_out()            noexcept{}
  virtual void  do_when_mouse_acted(int  x, int  y) noexcept{}

  virtual void  render(image_cursor  cur) noexcept{}

  void  need_to_redraw() noexcept;
  void  need_to_reform() noexcept;


  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  const gbstd::string&  get_name(                        ) const noexcept{return m_name       ;}


  void     set_parent(widget*  parent)       noexcept{       m_parent = parent;}
  widget*  get_parent(               ) const noexcept{return m_parent         ;}


  void*  get_userdata() const noexcept{return m_userdata;}

  template<typename  T>  void  set_userdata(T*  ptr) noexcept
  {
    m_userdata = ptr;
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
  windows::window*  m_window=nullptr;

  std::vector<std::unique_ptr<widget>>  m_children;

  widget*  m_current=nullptr;

  bool  m_unique;

public:
  container(bool  unique=false) noexcept: m_unique(unique){}

  const char*  get_widget_name() const noexcept override{return "container";}

  bool  is_unique() const noexcept{return m_unique;}

  void  change_current_by_name(gbstd::string_view  name) noexcept;
  widget*  get_current() const noexcept{return m_current;}

  void  reform(point  base_pt) noexcept override;
  void  render(image_cursor  cur) noexcept override;

  widget*  scan_by_point(int  x, int  y) noexcept override;

  void              set_window(windows::window*  w)       noexcept{m_window = w;}
  windows::window*  get_window(                   ) const noexcept override{return m_window? m_window:widget::get_window();}

  void  append_child(widget*  child, int  x, int  y) noexcept;

  void  show_all() noexcept override;

  void  print(int  indent=0) const noexcept override;

};


enum class
event_kind
{
  cursor_got_in,
  cursor_got_out,
  mouse_acted,
};


class
wrapper: public widget
{
  std::unique_ptr<widget>  m_target;

  void  (*m_callback)(wrapper&,event_kind,int,int)=nullptr;

public:
  wrapper(widget*  target, void  (*callback)(wrapper&,event_kind,int,int)) noexcept:
  m_target(target), m_callback(callback){}

  const char*  get_widget_name() const noexcept override{return "wrapper";}

  void  do_when_cursor_got_in()             noexcept override;
  void  do_when_cursor_got_out()            noexcept override;
  void  do_when_mouse_acted(int  x, int  y) noexcept override;

  void  reform(point  base_pt) noexcept override;
  void  render(image_cursor  cur) noexcept override;

  void     set_target(widget*  target) noexcept;
  widget*  get_target(               ) const noexcept{return m_target.get();}

  void  show_all() noexcept override;

  void  print(int  indent=0) const noexcept override;

};




class
root
{
  container  m_container;

  widget*  m_current=nullptr;

public:
  container*  operator->() noexcept{return &m_container;}

  const widget*  get_current() const noexcept{return m_current;}

  void   react() noexcept;
  bool  update() noexcept;

};




class
label: public widget
{
  text_style  m_text_style=text_style(predefined::blue,predefined::white,predefined::blue,predefined::black);

  gbstd::u16string  m_text;

public:
  label(gbstd::string_view     sv) noexcept;
  label(gbstd::u16string_view  sv) noexcept;

  const char*  get_widget_name() const noexcept override{return "label";}

  void  set_text(gbstd::u16string_view  sv) noexcept;
  void  set_text(gbstd::string_view     sv) noexcept;

  void  set_text_sytle(const text_style&  style) noexcept;

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


class
icon
{
public:
  static constexpr int  size = 16;

private:
  color  m_data[size][size]={0};

public:
  icon(std::initializer_list<int>  ls) noexcept;
  icon(std::initializer_list<color>  ls) noexcept;

  void   set_color(int  x, int  y, color  i)       noexcept{       m_data[y][x] = i;}
  color  get_color(int  x, int  y          ) const noexcept{return m_data[y][x]    ;}

  void  print() const noexcept;

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

  void  render(image_cursor  cur) noexcept override;

  void  print(int  indent=0) const noexcept override;

  void  show_all() noexcept override;

};


class
radio_button: public wrapper
{
public:
  using callback_prototype = void  (*)(radio_button&  btn, uint32_t  old_state, uint32_t  new_state);

private:
  struct shared_data;

  shared_data*  m_data;

protected:
  uint32_t  m_bit_id;

  radio_button*  m_next=nullptr;

  static void  common_callback(wrapper&  wr, event_kind  k, int  x, int  y) noexcept;

  void  call(uint32_t  new_state) noexcept;

public:
  radio_button(widget*  target, callback_prototype  cb) noexcept;
  radio_button(widget*  target, radio_button&  first) noexcept;
 ~radio_button();

  const char*  get_widget_name() const noexcept override{return "radio_button";}

  bool  is_checked() const noexcept;

  virtual void  change_state() noexcept;

  uint32_t  get_bit_id() const noexcept{return m_bit_id;}

  uint32_t  get_state() const noexcept;

  void  render(image_cursor  cur) noexcept override;

};


class
check_button: public radio_button
{
public:
  using radio_button::radio_button;

  const char*  get_widget_name() const noexcept override{return "check_button";}

  void  change_state() noexcept override;

};




class
table_column: public container
{
public:
  table_column(std::initializer_list<widget*>  ls={}) noexcept{append(ls);}

  const char*  get_widget_name() const noexcept override{return "table_column";}

  void  reform(point  base_pt) noexcept override;

  void  append(widget*  w) noexcept;
  void  append(std::initializer_list<widget*>  ls) noexcept;

};


class
table_row: public container
{
public:
  table_row(std::initializer_list<widget*>  ls={}) noexcept{append(ls);}

  const char*  get_widget_name() const noexcept override{return "table_row";}

  void  reform(point  base_pt) noexcept override;

  void  append(widget*  w) noexcept;
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


class
text_roll: public widget
{
  character_queue  m_queue;

  text  m_text;

  int  m_start_line=0;

  bool  m_full_lined=false;
  bool  m_needing_to_fill=false;

  point  m_cursor;

  text_style  m_style;

public:
  text_roll(int  w, int  h) noexcept;

  const char*  get_widget_name() const noexcept override{return "text_roll";}

  character_queue&  get_queue() noexcept{return m_queue;}

  bool  is_needing_to_linefeed() const noexcept{return m_cursor.x >= m_text.get_width();}

  void  clear() noexcept;

  void  linefeed() noexcept;

  void  type() noexcept;

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

};




using menu_item_reactor  = bool  (*)(                   point  index);
using menu_item_renderer = void  (*)(image_cursor  cur, point  index);


struct
menu_parameter
{
  int  table_width =0;
  int  table_height=0;

  int  item_width =0;
  int  item_height=0;

  menu_item_reactor   item_reactor =nullptr;
  menu_item_renderer  item_renderer=nullptr;

};


class
menu: public widget
{
  menu_parameter  m_parameter;

public:
  menu(const menu_parameter&  para) noexcept: m_parameter(para){}

  void  do_when_mouse_acted(int  x, int  y) noexcept override;

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


widget*  create_radio_menu(std::initializer_list<widget*>  ls, uint32_t  initial_state=0) noexcept;
widget*  create_check_menu(std::initializer_list<widget*>  ls, uint32_t  initial_state=0) noexcept;


}


using widgets::widget;
using widgets::container;


}


#endif




