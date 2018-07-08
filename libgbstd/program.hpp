#ifndef gbstd_program_HPP
#define gbstd_program_HPP


#include"libgbstd/string.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/board.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/control_device.hpp"
#include<new>
#include<utility>
#include<array>
#include<cstdint>


namespace gbstd{
namespace programs{


class program;


class
value
{
  enum class kind{
    null,
    integer,
    string,
    real_number,
    keyboard,

  } m_kind=kind::null;

  union data{
    int            i;
    gbstd::string  s;
    double         r;
    keyboard     kbd;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(int  i) noexcept{*this = i;}
  value(gbstd::string_view  sv) noexcept{*this = sv;}
  value(keyboard  kbd)      noexcept{*this = kbd;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(int  i) noexcept;
  value&  operator=(gbstd::string_view  sv) noexcept;
  value&  operator=(keyboard  kbd) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_integer() const noexcept{return m_kind == kind::integer;}
  bool  is_string()  const noexcept{return m_kind == kind::string;}
  bool  is_keyboard() const noexcept{return m_kind == kind::keyboard;}

  int                   get_integer() const noexcept{return m_data.i;}
  const gbstd::string&  get_string()  const noexcept{return m_data.s;}
  keyboard              get_keyboard()  const noexcept{return m_data.kbd;}

  void  print() const noexcept;

};


class
process
{
  program*  m_program;

  gbstd::string  m_name;

  uint32_t  m_pc;

  uint32_t  m_calling_count;

  value  m_end_value;

  bool  m_ended;
  bool  m_removed;

  void  (*m_callback)(process&  prc, void*  data)=nullptr;

  void*  m_data=nullptr;

public:
  process(                      ) noexcept{}
  process(gbstd::string_view  sv) noexcept: m_name(sv){}

  virtual ~process(){}

  bool  is_ended()   const noexcept{return m_ended;}
  bool  is_removed() const noexcept{return m_removed;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}

  void  set_pc(int  v  ) noexcept{m_pc  = v;}
  void  add_pc(int  v=1) noexcept{m_pc += v;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void  reset(programs::program&  program) noexcept;

  void          set_end_value(value&&  v)       noexcept{       m_end_value = std::move(v);}
  const value&  get_end_value(          ) const noexcept{return m_end_value               ;}

  void  call(process&  prc) noexcept;

  void  end(value  v=value()) noexcept;

  void  remove() noexcept;

  void  ready_to_step() noexcept{m_calling_count = 0;}

  template<typename  PRC, typename  T>
  void  set_callback(void  (*callback)(PRC&  prc, T*  data), T*  data) noexcept
  {
      if(std::is_base_of<process,PRC>::value)
      {
        m_callback = reinterpret_cast<void(*)(process&,void*)>(callback);
        m_data     = data;
      }
  }


  void  execute_callback() noexcept{if(m_callback){m_callback(*this,m_data);}}

  virtual void  step() noexcept{}

};


class
program
{
  std::vector<process*>  m_stack;

  process*  m_top=nullptr;

public:
  ~program(){clear();}

  void  clear() noexcept;

  operator bool() const noexcept{return m_top;}

  void  push(process&  prc) noexcept;

  void  pop(value  v=value()) noexcept;

  void  step() noexcept;

};




class
sleep: public process
{
  uint32_t  m_time;

public:
  sleep(uint32_t  t=0) noexcept;

  void  step() noexcept override;

};



class
chooser: public process
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
indication: public process
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
  indication(rectangle  rect=rectangle()) noexcept;

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


using program = programs::program;
using process = programs::process;


extern uint32_t  g_time;

extern keyboard  g_previous_input;
extern keyboard  g_modified_input;
extern keyboard           g_input;

extern spaces::space<spaces::object>  g_screen_object_space;


extern image
g_misc_image;


}




#endif




