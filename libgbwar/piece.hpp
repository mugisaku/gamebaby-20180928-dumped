#ifndef gbwar_piece_HPP
#define gbwar_piece_HPP


#include"libgbstd/space.hpp"
#include"libgbstd/direction.hpp"
#include"libgbstd/program.hpp"
#include"libgbwar/board.hpp"


enum class
team_color
{
  blue, red

};


class piece;


class
piece_object: public spaces::object
{
  piece*  m_target=nullptr;

  int  m_pause_counter=0;

  uint32_t  m_last_time=0;

public:
  piece_object(piece&  p) noexcept: m_target(&p){}

  void  set_target(piece*  p) noexcept{m_target = p;}

  void  update_core() noexcept override;

  void  render(point  offset, image_cursor  cur) noexcept override;

};




class
piece
{
  piece_object  m_object;

  gbstd::string  m_name;

  int  m_hp=0;
  int  m_hp_max=0;

  int  m_mv=0;
  int  m_mv_max=0;

  direction  m_direction;

  team_color  m_team_color;

  square*  m_square=nullptr;

public:
  piece() noexcept: m_object(*this){}

  operator piece_object&() noexcept{return m_object;}

  piece_object&  get_object() noexcept{return m_object;}

  void                  set_name(gbstd::string_view  sv)       noexcept{       m_name = sv;}
  const gbstd::string&  get_name(                      ) const noexcept{return m_name     ;}

  void  reset_hp() noexcept{m_hp = m_hp_max;}

  int   get_hp() const noexcept{return m_hp;}

  void  set_hp(int  v) noexcept{m_hp  = v;}
  void  add_hp(int  v) noexcept{m_hp += v;}

  int  get_hp_max() const noexcept{return m_hp_max;}

  void  set_hp_max(int  v) noexcept{m_hp_max  = v;}
  void  add_hp_max(int  v) noexcept{m_hp_max += v;}


  void  reset_mv() noexcept{m_mv = m_mv_max;}

  int   get_mv() const noexcept{return m_mv;}

  void  set_mv(int  v) noexcept{m_mv  = v;}
  void  add_mv(int  v) noexcept{m_mv += v;}

  int  get_mv_max() const noexcept{return m_mv_max;}

  void  set_mv_max(int  v) noexcept{m_mv_max  = v;}
  void  add_mv_max(int  v) noexcept{m_mv_max += v;}


  direction  get_direction(              ) const noexcept{return m_direction      ;}
  void       set_direction(direction  dir)       noexcept{       m_direction = dir;}

  square*  get_square(           ) const noexcept{return m_square     ;}
  void     set_square(square*  sq)       noexcept;

  team_color  get_team_color() const noexcept{return m_team_color;}


  void   seek_route() const noexcept;

  int  get_mv_consumption(const square&  sq) const noexcept
  {
    return 1;
  }


  static image  g_image;

};


class
piece_info: public spaces::object
{
  piece*  m_target=nullptr;

public:
  piece_info() noexcept{}


  void    set_target(piece*  p)       noexcept{       m_target = p;}
  piece*  get_target(         ) const noexcept{return m_target    ;}

  void  update_core() noexcept override;

  void  render(point  offset, image_cursor  cur) noexcept override;

};




#endif




