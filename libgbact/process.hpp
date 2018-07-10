#ifndef gbact_processs_hpp
#define gbact_processs_hpp


#include"libgbact/character.hpp"
#include"libgbact/board.hpp"
#include"libgbact/stage.hpp"
#include"libgbstd/program.hpp"
#include<array>


extern int  g_screen_width ;
extern int  g_screen_height;

extern images::image  g_bg_image;


extern std::vector<gbact::stages::stage>
g_stage_table;


extern int
g_stage_index;


namespace gbact{
namespace processes{


class
edit: public programs::process
{
  class
  entry_display: public spaces::object
  {
  public:
    void  render(point  offset, image_cursor  cur) noexcept override;

  } m_entry_display;


  bool  m_lock=false;

  programs::indication  m_src_indication;
  programs::indication  m_dst_indication;

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

  static void  src_callback(programs::indication&  ctx, edit*  ed_prc) noexcept;
  static void  dst_callback(programs::indication&  ctx, edit*  ed_prc) noexcept;

public:
  void  step() noexcept override;

  void  clean() noexcept;

};


class
play: public programs::process
{
  programs::chooser  m_chooser;

  characters::lady_monitor  m_lady_monitor;

  character_set  m_character_set;

  uint32_t  m_timer;

  static void  callback(programs::indication&  ind, play*  play_prc) noexcept;

  void   load_stage() noexcept;
  void  clean_stage() noexcept;

public:
  void  step() noexcept override;

  void  clean() noexcept;

};


}
}




#endif




