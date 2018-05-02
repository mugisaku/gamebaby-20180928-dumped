#ifndef gbstd_widget_hpp_was_included
#define gbstd_widget_hpp_was_included


#include"libgbstd/image.hpp"
#include"libgbstd/control_device.hpp"
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
class root;


class
widget
{
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


  void*  get_userdata() const noexcept{return m_userdata;}

  void  set_userdata(void*  ptr) noexcept
  {
    m_userdata = ptr;
  }

  template<typename  T>  void  set_userdata(T*  ptr) noexcept
  {
    m_userdata = ptr;
  }


  bool  test_by_absolute_point(point  pt) const noexcept;

  virtual widget*  scan_by_absolute_point(point  pt) noexcept{return test_by_absolute_point(pt)? this:nullptr;}

  const point&  get_absolute_point() const noexcept{return m_absolute_point;}
  const point&  get_relative_point() const noexcept{return m_relative_point;}

  void  set_relative_point(point  pt) noexcept{m_relative_point = pt;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}


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
background_style
{
  enum class kind{
    single_color,
    stripe,
  } m_kind=kind::single_color;

  color   m_first_color;
  color  m_second_color;

  int  m_interval;

public:
  background_style() noexcept{}
  background_style(color  color1) noexcept: m_first_color(color1){}

  background_style(color  color1, color  color2, int  interval) noexcept:
  m_kind(kind::stripe), m_first_color(color1), m_second_color(color2), m_interval(interval){}

  void  be_single_color() noexcept{m_kind = kind::single_color;}
  void  be_stripe()       noexcept{m_kind = kind::stripe;}

  bool  is_single_color() const noexcept{return m_kind == kind::single_color;}
  bool  is_stripe() const noexcept{return m_kind == kind::stripe;}

  void  set_interval(int  v)       noexcept{       m_interval = v;}
  int   get_interval(      ) const noexcept{return m_interval    ;}

  void    set_first_color(images::color  color)       noexcept{       m_first_color = color;}
  color   get_first_color(                    ) const noexcept{return m_first_color        ;}

  void   set_second_color(images::color  color)       noexcept{       m_second_color = color;}
  color  get_second_color(                    ) const noexcept{return m_second_color        ;}

  void  render(int  x, int  y, int  w, int  h, image_cursor&  cur) const noexcept
  {
      switch(m_kind)
      {
    case(kind::single_color): cur.fill_rectangle(m_first_color,x,y,w,h);break;
    case(kind::stripe      ): cur.draw_stripe_rectangle(m_first_color,m_second_color,m_interval,x,y,w,h);break;
      }
  }

};


class
node: public widget
{
  static background_style  m_default_background_style;

protected:
  std::unique_ptr<widget>  m_target;

  widget*  m_current=nullptr;

  const background_style*  m_background_style=&m_default_background_style;

public:
  node(widget*  target=nullptr) noexcept:
  m_target(target)
  {}

  const char*  get_widget_name() const noexcept override{return "node";}

  void  set_root(root*  r) noexcept override;

  const widget*  get_current() const noexcept{return m_current;}

  void  update() noexcept override;

  void  reform(point  base_pt) noexcept override;
  void  render(image_cursor  cur) noexcept override;

  void  cancel_current() noexcept;

  void  set_background_style(const background_style&  new_style) noexcept;

  void     set_target(widget*  target) noexcept;
  widget*  get_target(               ) const noexcept{return m_target.get();}

  void  show_all() noexcept override;

  void  print(int  indent=0) const noexcept override;

  static void  set_default_background_style(background_style  new_style) noexcept{m_default_background_style = new_style;}

};


class
frame: public node
{
  static text_style  m_default_text_style;

  gbstd::string  m_text;

  text_style  m_text_style=m_default_text_style;

  color  m_line_color;

  static color  m_default_line_color;

public:
  frame(widget*  target, gbstd::string_view  text) noexcept:
  node(target), m_text(text),
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
  static text_style  m_default_text_style;

  text_style  m_text_style=m_default_text_style;

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

public:
  dial(int  min, int  max, void  (*callback)(dial&,int,int)) noexcept;

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

  color  get_color() const noexcept{return m_color;}

  void  update_color() noexcept;

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




class
canvas: public widget
{
  static background_style  m_default_background_style;

  image*  m_image=nullptr;

  int  m_pixel_size=1;

  bool  m_grid=false;

  background_style  m_background_style=m_default_background_style;

  enum class mode{
    draw_dot,
    draw_line,
    draw_rectangle,
    fill_rectangle,
    fill_area,

  } m_mode=mode::draw_dot;


  drawing_recorder  m_recorder;

  color  m_drawing_color=color(0,0,0);

  int  m_pointing_count=0;

  point  m_a_point;
  point  m_b_point;

  rectangle  m_operation_rect;

  void  (*m_callback)(canvas&  cv)=nullptr;

public:
  canvas(){}
  canvas(image&  img, void  (*callback)(canvas&  cv)) noexcept: m_callback(callback){set_image(img);}
 ~canvas(){}

  int   get_pixel_size(      ) const noexcept{return m_pixel_size;}
  void  set_pixel_size(int  n)       noexcept;

  void   set_drawing_color(images::color  color)       noexcept{       m_drawing_color = color;}
  color  get_drawing_color(                    ) const noexcept{return m_drawing_color        ;}

  drawing_recorder&  get_drawing_recorder() noexcept{return m_recorder;}

  void    set_grid() noexcept;
  void  unset_grid() noexcept;

  void  change_mode_to_draw_dot()       noexcept{m_mode = mode::draw_dot;}
  void  change_mode_to_draw_line()      noexcept{m_mode = mode::draw_line;}
  void  change_mode_to_draw_rectangle() noexcept{m_mode = mode::draw_rectangle;}
  void  change_mode_to_fill_rectangle() noexcept{m_mode = mode::fill_rectangle;}
  void  change_mode_to_fill_area()      noexcept{m_mode = mode::fill_area;}

  void    set_image(image&  img)       noexcept;
  image*  get_image(           ) const noexcept{return m_image;}

  void  reform(point  base_pt) noexcept override;

  void  modify_dot(images::color  new_color, int  x, int  y) noexcept;

  void  revolve() noexcept;
  void  reverse_horizontally() noexcept;
  void  reverse_vertically() noexcept;
  void  mirror_vertically() noexcept;
  void  shift_up(bool  rotate) noexcept;
  void  shift_left(bool  rotate) noexcept;
  void  shift_right(bool  rotate) noexcept;
  void  shift_down(bool  rotate) noexcept;

  void  draw_line(images::color  color, point  a, point  b) noexcept;
  void  draw_rect(images::color  color, rectangle  rect) noexcept;
  void  fill_rect(images::color  color, rectangle  rect) noexcept;
  void  fill_area(images::color  color, point  pt) noexcept;

  widget*  create_color_maker() noexcept;
  widget*  create_tool_widget() noexcept;
  widget*  create_operation_widget() noexcept;

  void  cancel_drawing() noexcept;

  void  undo() noexcept;

  void  do_when_cursor_got_out() noexcept override{cancel_drawing();}

  void  update() noexcept override;

  void  render(image_cursor  cur) noexcept override;

};


using rbcb = radio_button::callback_prototype;
using wls = std::initializer_list<widget*>;

widget*  create_radio_menu(wls  ls, rbcb  cb, uint32_t  initial_state=0, void*  userdata=nullptr) noexcept;
widget*  create_check_menu(wls  ls, rbcb  cb, uint32_t  initial_state=0, void*  userdata=nullptr) noexcept;


}


using widgets::widget;


}


#endif




