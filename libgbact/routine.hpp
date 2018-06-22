#ifndef routine_hpp
#define routine_hpp


#include"libgbact/character.hpp"
#include"libgbact/stage.hpp"
#include"libgbstd/program.hpp"
#include<array>


extern int  g_screen_width ;
extern int  g_screen_height;

extern validity  g_object_space_validity;
extern validity  g_character_space_validity;
extern validity  g_board_view_validity;

extern images::image  g_bg_image;
extern images::image  g_misc_image;

extern boards::board
g_board;


extern boards::board_view
g_board_view;


namespace gbact{
namespace routines{


class
sleep_context: public programs::context
{
  uint32_t  m_time;

public:
  sleep_context(uint32_t  t=0) noexcept: m_time(g_time+t){}

  void  step() noexcept override;

};



class
chooser_context: public programs::context
{
  std::vector<gbstd::string>  m_text_list;

  std::array<spaces::text_object,8>  m_text_objects;

  spaces::image_object  m_cursor;

  int  m_index;
  
public:
  chooser_context(std::initializer_list<gbstd::string_view>  ls={}) noexcept;

  void  step() noexcept override;

  void  clean() noexcept;

};



class
indication_context: public programs::context
{
  spaces::image_object  m_hand_cursor;

  point  m_point;

  rectangle  m_rectangle;

  int  m_x_pooling=0;
  int  m_y_pooling=0;

  int  m_speed=3;

  uint32_t  m_wait_time=100;
  uint32_t  m_next_time=  0;

  void  (*m_callback)(indication_context&  ctx, void*  data)=nullptr;

  void*  m_data;

  void  update() noexcept;

public:
  indication_context(rectangle  rect=rectangle()) noexcept;

  void  initialize(rectangle  rect) noexcept;

  template<typename  T>
  void  set_callback(void  (*callback)(indication_context&  ctx, T*  data), T*  data) noexcept
  {
    m_callback = reinterpret_cast<void(*)(indication_context&,void*)>(callback);
    m_data     = data;
  }

  point  get_point(              ) const noexcept{return m_point-static_cast<const point&>(m_rectangle);}
  void   set_point(int  x, int  y)       noexcept;

  int   get_speed(      ) const noexcept{return m_speed    ;}
  void  set_speed(int  v)       noexcept{       m_speed = v;}

  int  get_x_pooling() const noexcept{return m_x_pooling;}
  int  get_y_pooling() const noexcept{return m_y_pooling;}

  void  show_hand_cursor() noexcept{m_hand_cursor.show();}
  void  hide_hand_cursor() noexcept{m_hand_cursor.hide();}

  void  clean() noexcept;

  const rectangle&  get_rectangle() const noexcept{return m_rectangle;}

  void  step() noexcept override;

};


class
edit_context: public programs::context
{
  class
  square_data_display: public spaces::object
  {
  public:
    void  render(point  offset, image_cursor  cur) noexcept override;

  } m_square_data_display;


  routines::indication_context  m_src_indication_context;
  routines::indication_context  m_dst_indication_context;

  rectangle  m_square_data_display_rectangle;

  spaces::image_object  m_character_image_object;

  spaces::image_object  m_src_square_cursor;
  spaces::image_object  m_dst_square_cursor;

  boards::square*  get_square() const noexcept;
  square_data*     get_square_data() const noexcept;

  void  update() noexcept;

  static void  src_callback(indication_context&  ctx, edit_context*  ed) noexcept;
  static void  dst_callback(indication_context&  ctx, edit_context*  ed) noexcept;

public:
  void  step() noexcept override;

  void  clean() noexcept;

};


class
play_context: public programs::context
{
  routines::chooser_context  m_chooser_context;

  bool  m_pausing;

  characters::lady  m_lady;
  characters::meat  m_meat;

  spaces::text_object  m_system_message;

  bool  m_set_meat_timer;

  uint32_t  m_next_meat_time;

  uint32_t  m_time;

public:
  void  step() noexcept override;

  void  clean() noexcept;

};


}
}


extern image  g_misc_image;




#endif




