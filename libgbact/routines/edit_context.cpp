#include"libgbact/routine.hpp"




namespace gbact{
namespace routines{




boards::square*
edit_context::
get_square() const noexcept
{
  auto  pt = m_indication_context.get_point();

    if(pt.y >= 48)
    {
      int  x = (pt.x   )/g_square_size;
      int  y = (pt.y-48)/g_square_size;

      return &g_board.get_square(x,y);
    }


  return nullptr;
}


square_data&
edit_context::
get_square_data() const noexcept
{
  auto  pt = point(m_item_cursor.get_base_point());

  int  x = pt.x/g_square_size;
  int  y = pt.y/g_square_size;

  return stages::g_square_data_set[x];
}


void
edit_context::
callback(indication_context&  ctx, edit_context*  ed) noexcept
{
  auto  sq = ed->get_square();

    if(sq)
    {
      sq->set_data(ed->get_square_data());
    }

  else
    {
      auto  pt = ctx.get_point();

      ed->m_item_cursor.set_base_point(pt.x/g_square_size*g_square_size,
                                       pt.y/g_square_size*g_square_size);
    }
}


void
edit_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      m_indication_context.initialize(rectangle(0,0,g_screen_width-24,g_screen_height-24));

      m_indication_context.set_callback(callback,this);

      m_bg_image_object = spaces::image_object(g_bg_image,rectangle(0,0,24*3,48),point());

      m_item_cursor = spaces::image_object(g_misc_image,rectangle(24,0,24,24),point());

      g_object_space.append(m_bg_image_object);
      g_object_space.append(m_item_cursor);

      set_pc(1);
      break;
  case(1):
      call(m_indication_context);

      set_pc(2);
      break;
  case(2):
        {
          auto  kbd = get_end_value().get_keyboard();

            if(kbd.test_n_button())
            {
            }
        }

      set_pc(1);
      break;
    }
}




}}




