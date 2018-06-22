#include"libgbact/routine.hpp"




namespace gbact{
namespace routines{




void
edit_context::square_data_display::
render(point  offset, image_cursor  cur) noexcept
{
  int  x = 0;

    for(auto&  dat: stages::g_square_data_set)
    {
      auto  pt = dat.get_image_point();

      images::paste(g_bg_image,rectangle(pt,g_square_size,g_square_size),cur+point(x,0));

      x += g_square_size;
    }
}




boards::square*
edit_context::
get_square() const noexcept
{
  auto  pt = m_dst_indication_context.get_point();

  auto&  bv_off = g_board_view.get_offset();

  int  x = (bv_off.x+pt.x)/g_square_size;
  int  y = (bv_off.y+pt.y)/g_square_size;

  return &g_board.get_square(x,y);
}


square_data*
edit_context::
get_square_data() const noexcept
{
  auto  pt = m_src_indication_context.get_point();

  int  x = pt.x/g_square_size;

  return &stages::g_square_data_set[x];
}


void
edit_context::
src_callback(indication_context&  ctx, edit_context*  ed) noexcept
{
    if(g_input.test_p_button() ||
       g_input.test_n_button())
    {
      ctx.end();
    }


  ed->update();
}


void
edit_context::
dst_callback(indication_context&  ctx, edit_context*  ed) noexcept
{
  g_board_view.add_offset(ctx.get_x_pooling(),
                          ctx.get_y_pooling());

  g_board_view.correct_offset();

  auto  pt = ctx.get_point();

    if(g_input.test_p_button())
    {
      auto  sq = ed->get_square();

        if(sq)
        {
          auto  dat = ed->get_square_data();

            if(dat)
            {
              sq->set_data(*dat);
            }
        }
    }

  else
    if(g_input.test_n_button())
    {
      ctx.end();
    }


  ed->update();
}


void
edit_context::
update() noexcept
{
  auto&  bv_off = g_board_view.get_offset();

  auto  src_pt =             (m_src_indication_context.get_point()/g_square_size*g_square_size);
  auto  dst_pt = point(0,48)+(m_dst_indication_context.get_point()/g_square_size*g_square_size);

  m_src_square_cursor.set_base_point(src_pt);
  m_dst_square_cursor.set_base_point(dst_pt);
}


void
edit_context::
clean() noexcept
{
  m_src_indication_context.clean();
  m_dst_indication_context.clean();

  m_square_data_display.die();
  m_src_square_cursor.die();
  m_dst_square_cursor.die();
}


void
edit_context::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      g_board_view.set_offset(0,0);

      m_square_data_display_rectangle = rectangle(0,0,(g_square_size*stages::g_square_data_set.size())-24,g_square_size-24);

      m_src_indication_context.initialize(m_square_data_display_rectangle);

      m_src_indication_context.set_callback(src_callback,this);

      m_src_indication_context.set_speed(g_square_size);

      m_dst_indication_context.initialize(rectangle(0,48,g_screen_width-24,g_screen_height-48-24));

      m_dst_indication_context.set_callback(dst_callback,this);

      m_dst_indication_context.set_speed(g_square_size);


      m_src_square_cursor = spaces::image_object(g_misc_image,rectangle(24,0,24,24),point());
      m_dst_square_cursor = spaces::image_object(g_misc_image,rectangle(24,0,24,24),point());


      m_src_indication_context.hide_hand_cursor();
      m_dst_indication_context.hide_hand_cursor();

      m_src_square_cursor.show();

      g_object_space.append(m_square_data_display);
      g_object_space.append(m_src_square_cursor);
      g_object_space.append(m_dst_square_cursor);

      set_pc(1);
      break;
  case(1):
      m_dst_square_cursor.hide();

      call(m_src_indication_context);

      set_pc(2);
      break;
  case(2):
        if(g_input.test_n_button())
        {
          end();
        }

      else
        if(!g_input.test_p_button())
        {
          m_dst_square_cursor.show();

          call(m_dst_indication_context);

          set_pc(1);
        }
      break;
    }
}




}}




