#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{


column_menu::
column_menu(const menu_base&  base, int  number_of_pages) noexcept:
m_base(base),
m_number_of_pages(number_of_pages)
{
  m_width  = m_base.get_item_width()  +16;
  m_height = m_base.get_image_height()+16;
}




int
column_menu::
get_item_index() const noexcept
{
  return (m_base.get_number_of_visible_rows()*m_page_index)+m_row_index;
}


void
column_menu::
change_item_width(int  n) noexcept
{
  m_base.get_renderer().set_item_width(8*n);

  m_width  = m_base.get_item_width()  +16;
  m_height = m_base.get_image_height()+16;

}


void
column_menu::
reset_cursor() noexcept
{
  m_page_index = 0;
   m_row_index = 0;
}




void
column_menu::
move_cursor_to_left()  noexcept
{
    if(m_page_index)
    {
      m_page_index -= 1;
    }
}


void
column_menu::
move_cursor_to_right() noexcept
{
    if(m_page_index < (m_number_of_pages-1))
    {
      m_page_index += 1;
    }
}


void
column_menu::
move_cursor_to_up()    noexcept
{
    if(m_row_index)
    {
      m_row_index -= 1;
    }
}


void
column_menu::
move_cursor_to_down()  noexcept
{
    if(m_row_index < (m_base.get_number_of_visible_rows()-1))
    {
      m_row_index += 1;
    }
}




void
column_menu::
change_number_of_rows(int  n) noexcept
{
  m_base.set_number_of_visible_rows(n);

  m_width  = m_base.get_item_width()  +16;
  m_height = m_base.get_image_height()+16;
}


void
column_menu::
render(image_cursor  cur) noexcept
{
  int  h = m_base.get_item_height();

  m_base.render(cur,(m_base.get_number_of_visible_rows()*m_page_index));


  cur.draw_rectangle(predefined::yellow,0,(h*m_row_index),m_base.get_item_width(),h);
}




}}




