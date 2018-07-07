#ifndef gbact_routine_hpp
#define gbact_routine_hpp


#include"libgbact/character.hpp"
#include"libgbact/stage.hpp"
#include"libgbstd/program.hpp"
#include"libgbstd/routine.hpp"
#include<array>


extern int  g_screen_width ;
extern int  g_screen_height;

extern int  g_board_width ;
extern int  g_board_height;

extern validity  g_board_view_validity;

extern images::image  g_bg_image;

extern board
g_board;


extern board_view
g_board_view;


extern std::vector<gbact::stages::stage>
g_stage_table;


extern int
g_stage_index;


namespace gbact{
namespace routines{


class
edit_context: public programs::context
{
  class
  entry_display: public spaces::object
  {
  public:
    void  render(point  offset, image_cursor  cur) noexcept override;

  } m_entry_display;


  bool  m_lock=false;

  gbstd::routines::indication_context  m_src_indication_context;
  gbstd::routines::indication_context  m_dst_indication_context;

  rectangle  m_display_rectangle;

  spaces::image_object  m_character_image_object;

  spaces::image_object  m_src_square_cursor;
  spaces::image_object  m_dst_square_cursor;


  spaces::image_object   m_lady_object;
  spaces::image_object   m_boy_object;

  std::vector<spaces::image_object>  m_object_table;



  point  get_square_index() const noexcept;
  prop  get_prop() const noexcept;

  void  initialize() noexcept;

  void  update() noexcept;

  static void  src_callback(gbstd::routines::indication_context&  ctx, edit_context*  ed) noexcept;
  static void  dst_callback(gbstd::routines::indication_context&  ctx, edit_context*  ed) noexcept;

public:
  void  step() noexcept override;

  void  clean() noexcept;

};


class
play_context: public programs::context
{
  gbstd::routines::chooser_context  m_chooser_context;

  characters::lady_monitor  m_lady_monitor;

  character_set  m_character_set;

  uint32_t  m_timer;

  static void  callback(gbstd::routines::indication_context&  ctx, play_context*  play) noexcept;

  void   load_stage() noexcept;
  void  clean_stage() noexcept;

public:
  void  step() noexcept override;

  void  clean() noexcept;

};


}
}


extern image  g_misc_image;




#endif




