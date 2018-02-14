#ifndef GMBB_Menu_HPP_INCLUDED
#define GMBB_Menu_HPP_INCLUDED


#include<cstdint>
#include<algorithm>
#include"libgbstd/image.hpp"
#include"libgbstd/window.hpp"




namespace gbstd{
namespace menus{


using callback = void  (*)(image&  dst, images::point  point, int  index);


class
menu_item_renderer
{
  int  m_item_width;
  int  m_item_height;

  callback  m_callback;

public:
  constexpr menu_item_renderer(int  w, int  h, callback  cb) noexcept:
  m_item_width(w),
  m_item_height(h),
  m_callback(cb){}

  constexpr int  get_item_width()  const noexcept{return m_item_width;}
  constexpr int  get_item_height() const noexcept{return m_item_height;}

  void  set_item_width( int  n) noexcept{m_item_width  = n;}
  void  set_item_height(int  n) noexcept{m_item_height = n;}

  void  operator()(image&  dst, images::point  base_point, images::point  item_position, int  index) const noexcept
  {
    int  x = base_point.x+(m_item_width *item_position.x);
    int  y = base_point.y+(m_item_height*item_position.y);

    m_callback(dst,point(x,y),index);
  }

};


class
menu_base
{
  menu_item_renderer  m_renderer;

  int  m_number_of_visible_rows;

public:
  constexpr menu_base(const menu_item_renderer&  rend, int  visible_rows) noexcept:
  m_renderer(rend),
  m_number_of_visible_rows(visible_rows){}

  menu_item_renderer&  get_renderer() noexcept{return m_renderer;}

  constexpr int  get_item_width()  const noexcept{return m_renderer.get_item_width();}
  constexpr int  get_item_height() const noexcept{return m_renderer.get_item_height();}

  constexpr int  get_number_of_visible_rows() const noexcept{return m_number_of_visible_rows;}
  constexpr int  get_image_height() const noexcept{return get_item_height()*m_number_of_visible_rows;}

  void  set_number_of_visible_rows(int  n) noexcept{m_number_of_visible_rows = n;}

  void  render(image&  dst, point  dst_point, int  start_index, int  number_of_columns=1) const noexcept;

};


class
column_menu: public window
{
  menu_base  m_base;

  int  m_page_index=0;
  int   m_row_index=0;

  int  m_number_of_pages;

public:
  column_menu(const menu_base&  base, int  number_of_pages, point  pt) noexcept;

  int  get_item_index() const noexcept;

  void  move_cursor_to_left()  noexcept;
  void  move_cursor_to_right() noexcept;
  void  move_cursor_to_up()    noexcept;
  void  move_cursor_to_down()  noexcept;

  const menu_base&  get_base() const noexcept{return m_base;}

  void  change_item_width(int  n) noexcept;

  void  change_number_of_rows(int  n) noexcept;

  void  reset_cursor() noexcept;

  void  render(image&  dst, point  offset) const noexcept override;

};


class
roll_menu: public window
{
  menu_base  m_base;

  int  m_number_of_columns;
  int     m_number_of_rows;

  int  m_y_base=0;

  point  m_cursor;

public:
  roll_menu(const menu_base&  base, int  col_n, int  row_n, point  pt) noexcept;

  int  get_item_index() const noexcept{return (m_number_of_columns*(m_y_base+m_cursor.y))+m_cursor.x;}

  void  move_cursor_to_left()  noexcept;
  void  move_cursor_to_right() noexcept;
  void  move_cursor_to_up()    noexcept;
  void  move_cursor_to_down()  noexcept;

  void  reset_cursor() noexcept;

  void  render(image&  dst, point  offset) const noexcept override;

};


}


using menus::menu_base;
using menus::menu_item_renderer;
using menus::roll_menu;
using menus::column_menu;


}




#endif




