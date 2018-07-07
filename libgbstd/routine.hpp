#ifndef gbstd_routine_hpp
#define gbstd_routine_hpp


#include"libgbstd/program.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/image.hpp"
#include<array>


namespace gbstd{


extern uint32_t  g_time;

extern keyboard  g_previous_input;
extern keyboard  g_modified_input;
extern keyboard           g_input;

extern spaces::space<spaces::object>  g_screen_object_space;


extern image
g_misc_image;


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
  std::vector<spaces::text_object>  m_text_objects;

  spaces::image_object  m_cursor;

  int  m_index;

public:
  void  initialize(std::initializer_list<gbstd::string_view>  ls, int  x, int  y) noexcept;

  int  get_index() const noexcept{return m_index;}

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

  void  update() noexcept;

public:
  indication_context(rectangle  rect=rectangle()) noexcept;

  void  initialize(rectangle  rect) noexcept;

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




}
}




#endif




