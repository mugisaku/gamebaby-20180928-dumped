#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{


roll_menu::
roll_menu(const menu_base&  base, int  col_n, int  row_n) noexcept:
m_base(base),
m_number_of_columns(col_n),
m_number_of_rows(row_n)
{
  m_width  = (m_base.get_item_width()*m_number_of_columns+16);
  m_height = (m_base.get_image_height()                  +16);
}




void
roll_menu::
reset_cursor() noexcept
{
  m_y_base = 0;

  m_cursor = point();
}




void
roll_menu::
move_cursor_to_left()  noexcept
{
    if(m_cursor.x)
    {
      m_cursor.x -= 1;
    }
}


void
roll_menu::
move_cursor_to_right() noexcept
{
   if(m_cursor.x < (m_number_of_columns-1))
   {
     m_cursor.x += 1;
   }
}


void
roll_menu::
move_cursor_to_up()    noexcept
{
       if(m_cursor.y){m_cursor.y -= 1;}
  else if(  m_y_base){m_y_base   -= 1;}
}


void
roll_menu::
move_cursor_to_down()  noexcept
{
  const int  y_base_end = (m_y_base+m_base.get_number_of_visible_rows());

       if(m_cursor.y < (m_base.get_number_of_visible_rows()-1)){m_cursor.y += 1;}
  else if(y_base_end < (m_number_of_rows                   -1)){m_y_base   += 1;}
}




void
roll_menu::
render(image_cursor  cur) noexcept
{
  int  w = m_base.get_item_width();
  int  h = m_base.get_item_height();

  m_base.render(cur,(m_number_of_columns*m_y_base),m_number_of_columns);

  cur.draw_rectangle(predefined_color::yellow,(w*m_cursor.x),
                                        (h*m_cursor.y),w,h);
}




}}




