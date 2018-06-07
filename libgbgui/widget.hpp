#ifndef gbgui_widget_hpp_was_included
#define gbgui_widget_hpp_was_included


#include"libgbstd/image.hpp"
#include"libgbstd/control_device.hpp"
#include"libgbstd/string.hpp"
#include"libgbstd/text.hpp"
#include"libgbgui/icon.hpp"
#include"libgbstd/utility.hpp"
#include<initializer_list>
#include<memory>


namespace gbgui{


using namespace gbstd;


namespace windows{
class window;
}


namespace widgets{


class widget;
class container;
class root;




class
widget
{
  static background_style  m_default_background_style;

protected:
  gbstd::string  m_name;

  uint32_t  m_flags=0;

  widget*  m_parent=nullptr;
  widget*    m_next=nullptr;

  root*  m_root=nullptr;

  point  m_absolute_point;
  point  m_relative_point;

  int  m_width =0;
  int  m_height=0;

  const background_style*  m_background_style=&m_default_background_style;

  void*  m_userdata=nullptr;

public:
  struct flags{
    static constexpr uint32_t  shown            = 0x0001;
    static constexpr uint32_t  needed_to_reform = 0x0002;
    static constexpr uint32_t  needed_to_redraw = 0x0004;
  };


  widget(int  w=1, int  h=1) noexcept: m_width(w), m_height(h){need_to_reform();}
  widget(const widget&   rhs) noexcept=delete;
  widget(      widget&&  rhs) noexcept=delete;
  virtual ~widget(){}

  widget&  operator=(const widget&   rhs) noexcept=delete;
  widget&  operator=(      widget&&  rhs) noexcept=delete;

  virtual const char*  get_widget_name() const noexcept{return "widget";}

  virtual void  reform(point  base_pt) noexcept;

  void  redraw(image&  img) noexcept;

  virtual void  do_when_cursor_got_in() noexcept{}
  virtual void  do_when_cursor_got_out() noexcept{}
  virtual void  update() noexcept{}

  virtual void  render(image_cursor  cur) noexcept{}
  virtual void  render_background(image_cursor  cur) const noexcept;

  mouse  get_mouse() const noexcept;

  void  need_to_redraw() noexcept;
  void  need_to_reform() noexcept;


  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  const gbstd::string&  get_name(                        ) const noexcept{return m_name       ;}


  void     set_parent(widget*  parent)       noexcept{       m_parent = parent;}
  widget*  get_parent(               ) const noexcept{return m_parent         ;}


  void     set_next(widget*  w)       noexcept{       m_next = w;}
  widget*  get_next(          ) const noexcept{return m_next    ;}


  virtual void   set_root(root*  r)       noexcept{       m_root = r;}
  root*          get_root(        ) const noexcept{return m_root    ;}


  template<typename  T>T*  get_userdata() const noexcept{return static_cast<T*>(m_userdata);}
  void*  get_userdata() const noexcept{return m_userdata;}

  void  set_userdata(void*  ptr) noexcept
  {
    m_userdata = ptr;
  }

  template<typename  T>  void  set_userdata(T*  ptr) noexcept
  {
    m_userdata = ptr;
  }

  static void  set_userdata(void*  data, std::initializer_list<widget*>  ls) noexcept;


  bool  test_by_absolute_point(point  pt) const noexcept;

  virtual widget*  scan_by_absolute_point(point  pt) noexcept{return test_by_absolute_point(pt)? this:nullptr;}

  const point&  get_absolute_point() const noexcept{return m_absolute_point;}
  const point&  get_relative_point() const noexcept{return m_relative_point;}

  void  set_relative_point(point  pt) noexcept{m_relative_point = pt;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_width( int  v) noexcept{ m_width = v;}
  void  set_height(int  v) noexcept{m_height = v;}


  void  set_style(const background_style&  new_style) noexcept;

  const background_style&  get_background_style() const noexcept{return *m_background_style;}

  static void  set_default_background_style(background_style  new_style) noexcept{m_default_background_style = new_style;}


  void    set_flag(uint32_t  v) noexcept{m_flags |=  v;}
  void  unset_flag(uint32_t  v) noexcept{m_flags &= ~v;}
  bool   test_flag(uint32_t  v) const noexcept{return m_flags&v;}

  void  notify_flag(uint32_t  v) noexcept;


  void  show() noexcept{  set_flag(flags::shown);}
  void  hide() noexcept{unset_flag(flags::shown);}

  void  reform_if_needed(point  base_pt) noexcept;

  virtual void  show_all() noexcept{show();}

  bool  is_shown() const noexcept{return test_flag(flags::shown);}

  bool  is_needed_to_reform() const noexcept{return test_flag(flags::needed_to_reform);}
  bool  is_needed_to_redraw() const noexcept{return test_flag(flags::needed_to_redraw);}

  virtual void  print(int  indent=0) const noexcept;

};




class
container: public widget
{
protected:
  std::vector<std::unique_ptr<widget>>  m_children;

  widget*  m_current=nullptr;

  bool  m_unique;

public:
  container(bool  unique=false) noexcept: m_unique(unique){}

  const char*  get_widget_name() const noexcept override{return "container";}

  void  set_root(root*  r) noexcept override;

  bool  is_unique() const noexcept{return m_unique;}

  void  change_current_by_name(gbstd::string_view  name) noexcept;
  widget*  get_current() const noexcept{return m_current;}

  void  do_when_cursor_got_in()  noexcept override;
  void  do_when_cursor_got_out() noexcept override;

  void  update() noexcept override;

  void  reform(point  base_pt) noexcept override;
  void  render(image_cursor  cur) noexcept override;

  widget*  scan_by_absolute_point(point  pt) noexcept override;

  void  append_child(widget*  child, int  x=0, int  y=0) noexcept;

  void  show_all() noexcept override;

  void  print(int  indent=0) const noexcept override;

};




class
node: public widget
{

protected:
  std::unique_ptr<widget>  m_target;

  widget*  m_current=nullptr;

  void  check_by_mouse(control_devices::mouse&  mouse) noexcept;

public:
  node(widget*  target=nullptr) noexcept:
  m_target(target)
  {}

  const char*  get_widget_name() const noexcept override{return "node";}

  void  set_root(root*  r) noexcept override;

  const widget*  get_current() const noexcept{return m_current;}

  void  do_when_cursor_got_in()  noexcept override;
  void  do_when_cursor_got_out() noexcept override;

  void  update() noexcept override;

  void  reform(point  base_pt) noexcept override;
  void  render(image_cursor  cur) noexcept override;

  void  cancel_current() noexcept;

  void     set_target(widget*  target) noexcept;
  widget*  get_target(               ) const noexcept{return m_target.get();}

  void  show_all() noexcept override;

  void  print(int  indent=0) const noexcept override;

};


class
frame: public node
{
  gbstd::string  m_text;

  text_style  m_text_style;

  color  m_line_color;

  static color  m_default_line_color;

public:
  frame(widget*  target, gbstd::string_view  text, const text_style&  style=styles::b_white_based_text_style) noexcept:
  node(target), m_text(text), m_text_style(style),
  m_line_color(m_default_line_color)
  {}

  const char*  get_widget_name() const noexcept override{return "frame";}

  void  reform(point  base_pt) noexcept override;

  void  set_line_color(color  new_color) noexcept;

  void  render(image_cursor  cur) noexcept override;

  static void  set_default_line_color(color  new_color) noexcept{m_default_line_color = new_color;}

};




class
root
{
  windows::window*  m_window=nullptr;

  node  m_node;

  point  m_offset;

  mouse  m_old_mouse;
  mouse  m_mouse;

  widget*  m_redrawing_first=nullptr;
  widget*  m_redrawing_last =nullptr;

public:
  root() noexcept{}
  root(windows::window&  win, widget*  wid) noexcept: m_window(&win), m_node(wid){}

  void          set_offset(point  pt)       noexcept{       m_offset = pt;}
  const point&  get_offset(         ) const noexcept{return m_offset     ;}

  void  set_node_target(widget*  w) noexcept;

  node&  get_node() noexcept{return m_node;}

  mouse&  get_mouse() noexcept{return m_mouse;}

  bool  is_mouse_moved() const noexcept{return m_mouse.point != m_old_mouse.point;}

  void  push_widget_that_needed_to_redraw(widget&  w) noexcept;

  void  put_down() noexcept{m_node.set_root(this);}

  void  react(const control_device&  condev) noexcept;

  void  redraw(image&  img) noexcept;
  bool  redraw_only_needed_widgets(image&  img) noexcept;

};




class
label: public widget
{
  text_style  m_text_style;

  gbstd::u16string  m_text;

public:
  label(gbstd::string_view     sv, const text_style&  ts=styles::b_black_based_text_style) noexcept;
  label(gbstd::u16string_view  sv, const text_style&  ts=styles::b_black_based_text_style) noexcept;

  const char*  get_widget_name() const noexcept override{return "label";}

  void  set_text(gbstd::u16string_view  sv) noexcept;
  void  set_text(gbstd::string_view     sv) noexcept;

  void  modify_text(gbstd::u16string_view  sv) noexcept;
  void  modify_text(gbstd::string_view     sv) noexcept;

  void  set_style(const text_style&  style) noexcept;

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


class
icon_selector: public widget
{
  const icon**  m_icons;

  int  m_current=0;

public:
  icon_selector(const icon**  icons) noexcept: widget(icon::size,icon::size), m_icons(icons){}

  const icon*  operator*() const noexcept{return m_icons[m_current];}

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

  void  do_when_cursor_got_out() noexcept override;

  void  update() noexcept override;

  void  render(image_cursor  cur) noexcept override;

  void  print(int  indent=0) const noexcept override;

  void  show_all() noexcept override;

};


class
radio_button: public node
{
public:
  using callback_prototype = void  (*)(radio_button&  btn, uint32_t  old_state, uint32_t  new_state);

private:
  struct shared_data;

  shared_data*  m_data;

  radio_button*  m_next=nullptr;

protected:
  uint32_t  m_bit_id;

  icon_selector*  m_icons;

  virtual const icon**  get_icons() const noexcept;

  void  call(uint32_t  new_state) noexcept;

public:
  radio_button(widget*  target, callback_prototype  cb) noexcept;
  radio_button(widget*  target, radio_button&  first) noexcept;
 ~radio_button();

  const char*  get_widget_name() const noexcept override{return "radio_button";}

  void  update() noexcept override;

  bool  is_checked() const noexcept;

  virtual void  change_state() noexcept;

  uint32_t  get_bit_id() const noexcept{return m_bit_id;}

  uint32_t  get_state(            ) const noexcept;
  void      set_state(uint32_t  st)       noexcept;

};


class
check_button: public radio_button
{
  const icon**  get_icons() const noexcept override;

public:
  using radio_button::radio_button;

  const char*  get_widget_name() const noexcept override{return "check_button";}

  void  change_state() noexcept override;

};




class
table_column: public container
{
public:
  table_column(std::initializer_list<widget*>  ls={}) noexcept{append_child(ls);}

  const char*  get_widget_name() const noexcept override{return "table_column";}

  void  reform(point  base_pt) noexcept override;

  void  append_child(std::initializer_list<widget*>  ls) noexcept;

};


class
table_row: public container
{
public:
  table_row(std::initializer_list<widget*>  ls={}) noexcept{append_child(ls);}

  const char*  get_widget_name() const noexcept override{return "table_row";}

  void  reform(point  base_pt) noexcept override;

  void  append_child(std::initializer_list<widget*>  ls) noexcept;

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

  void  update_label() noexcept;

public:
  dial(int  min, int  max, void  (*callback)(dial&,int,int)) noexcept;

  void  set_current(int  v) noexcept;

  int  get_current() const noexcept{return m_current;}
  int  get_min() const noexcept{return m_min;}
  int  get_max() const noexcept{return m_max;}

};


class
color_maker: public table_column
{
  dial*  m_r_dial;
  dial*  m_g_dial;
  dial*  m_b_dial;

  widget*  m_sample;

  void  (*m_callback)(color_maker&  cm, color  new_color)=nullptr;

  color  m_color;

  static void  update_color_internal(dial&  dial, int  old_value, int  new_value) noexcept;

public:
  color_maker(void  (*callback)(color_maker&,color)) noexcept;

  void   set_color(colors::color  color)       noexcept;
  color  get_color(                    ) const noexcept{return m_color;}

  void  update_color() noexcept;

};


class
color_holder: public widget
{
  int  m_index=0;

  std::vector<color>  m_colors;

  void  (*m_callback)(color_holder&  cm, color  new_color)=nullptr;

public:
  color_holder(int  size, void  (*callback)(color_holder&,color)) noexcept;
  color_holder(std::initializer_list<color>  colors, void  (*callback)(color_holder&,color)) noexcept;

  int  get_index() const noexcept{return m_index;}

  void    set_color(colors::color  color) noexcept;
  color&  get_color(                    ) noexcept{return m_colors[m_index];}

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

  void  update() noexcept override;

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




class menu;

using menu_item_reactor  = void  (*)(widgets::menu&  menu, point  index, mouse_button  left, mouse_button  right);
using menu_item_renderer = void  (*)(widgets::menu&  menu, point  index, image_cursor  cur);


struct
menu_item_parameter
{
  int  width =0;
  int  height=0;

  menu_item_reactor   reactor =nullptr;
  menu_item_renderer  renderer=nullptr;

};


class
menu: public widget
{
  menu_item_parameter  m_parameter;

  int  m_table_width ;
  int  m_table_height;

public:
  menu(const menu_item_parameter&  para, int  table_width, int  table_height) noexcept:
  m_parameter(para), m_table_width(table_width), m_table_height(table_height){}

  void  set_item_size(int  w, int  h) noexcept;

  void  update() noexcept override;

  void  reform(point  base_pt) noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


using rbcb = radio_button::callback_prototype;
using wls = std::initializer_list<widget*>;

widget*  create_radio_menu(wls  ls, rbcb  cb, uint32_t  initial_state=0, void*  userdata=nullptr) noexcept;
widget*  create_check_menu(wls  ls, rbcb  cb, uint32_t  initial_state=0, void*  userdata=nullptr) noexcept;


}


using widgets::widget;
using widgets::create_radio_menu;
using widgets::create_check_menu;


}


#endif




