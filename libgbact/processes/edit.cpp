#include"libgbact/process.hpp"




namespace gbact{
namespace processes{




namespace{
struct
entry
{
  prop  m_prop;

  const image*  m_image;

  rectangle  m_image_rectangle;

};


const entry
g_entries[] = 
{
  {prop(0,0),&g_bg_image,rectangle(24*0,24*0,24,24)},
  {prop(1,0),&g_bg_image,rectangle(24*0,24*1,24,24)},
  {prop(2,0),&g_bg_image,rectangle(24*2,24*0,24,24)},
  {prop(3,0),&g_bg_image,rectangle(24*1,24*0,24,24)},
  {prop(4,0),&g_bg_image,rectangle(24*1,24*1,24,24)},

  {prop(0,1),&characters::g_image,rectangle(point(24* 0,48*1   ),24,48)},
  {prop(0,2),&characters::g_image,rectangle(point(24*10,48*0   ),24,48)},
  {prop(0,3),&characters::g_image,rectangle(point(24*11,48*0+24),24,24)},
  {prop(0,4),&characters::g_image,rectangle(point(24*11,48*1   ),24,48)},

};
}


void
edit::entry_display::
render(point  offset, image_cursor  cur) noexcept
{
  int  x = 0;

    for(auto&  ent: g_entries)
    {
      images::paste(*ent.m_image,ent.m_image_rectangle,cur+point(x,0));

      x += square::size;
    }
}




point
edit::
get_square_index() const noexcept
{
  auto  pt = m_dst_indication.get_point();

  auto&  bv_off = g_board_view.get_offset();

  int  x = (bv_off.x+pt.x)/square::size;
  int  y = (bv_off.y+pt.y)/square::size;

  return point(x,y);
}


prop
edit::
get_prop() const noexcept
{
  auto  pt = m_src_indication.get_point();

  int  x = pt.x/square::size;

  return g_entries[x].m_prop;
}


void
edit::
src_callback(programs::indication&  ind, edit*  ed) noexcept
{
    if(g_input.test_p_button())
    {
      ind.end(1);
    }

  else
    if(g_input.test_n_button())
    {
      ind.end(0);
    }


  ed->update();
}


void
edit::
dst_callback(programs::indication&  ind, edit*  ed) noexcept
{
  g_board_view.add_offset(ind.get_x_pooling(),
                          ind.get_y_pooling());

  g_board_view.correct_offset();

    if(g_input.test_p_button())
    {
        if(ed->m_lock)
        {
          return;
        }


      auto  src_pr = ed->get_prop();

      ed->m_lock = src_pr.get_object_index();


      auto  pt = ed->get_square_index();

      auto&  stg = g_stage_table[g_stage_index];

      auto&  sq = g_board.get_square(pt.x,pt.y);

        if(src_pr.get_object_index() == 0)
        {
          sq.set_data(stages::g_square_data_set[src_pr.get_block_index()]);
        }


      stg.put_prop(src_pr,pt.x,pt.y);

      auto&  imgo = ed->m_object_table[g_board_width*pt.y+pt.x];

      auto  obji = stg.get_prop(pt.x,pt.y).get_object_index();

      point  obj_pt(square::size*pt.x,square::size*pt.y);

        if(obji == 0)
        {
          imgo.die();

          g_object_space.clean_dead_object();
        }

      else
        if(obji == 1)
        {
          obj_pt.y += 24;

          ed->m_lady_object.set_base_point(obj_pt);
        }

      else
        if(obji == 2)
        {
          obj_pt.y += 24;

          ed->m_boy_object.set_base_point(obj_pt);
        }

      else
        {
            if(obji == 3)
            {
              imgo = spaces::image_object(characters::g_image,g_entries[7].m_image_rectangle,point());
            }

          else
            if(obji == 4)
            {
              obj_pt.y += 24;

              imgo = spaces::image_object(characters::g_image,g_entries[8].m_image_rectangle,point(0,-48));
            }


          imgo.set_base_point(obj_pt);

          g_object_space.append(imgo);
        }
    }

  else
    {
      ed->m_lock = false;

        if(g_input.test_n_button())
        {
          ind.end();
        }
    }


  ed->update();
}


void
edit::
update() noexcept
{
  auto&  bv_off = g_board_view.get_offset();

  auto  src_pt =             (m_src_indication.get_point()/square::size*square::size);
  auto  dst_pt = point(0,48)+(m_dst_indication.get_point()/square::size*square::size);

  m_src_square_cursor.set_base_point(src_pt);
  m_dst_square_cursor.set_base_point(dst_pt);
}


void
edit::
initialize() noexcept
{
  m_object_table.resize(g_board_width*g_board_height);

  g_board_view.set_offset(0,0);

  m_display_rectangle = rectangle(0,0,(square::size*(stages::g_square_data_set.size()+4))-24,square::size-24);

  m_src_indication.initialize(m_display_rectangle);

  m_src_indication.set_callback(src_callback,this);

  m_src_indication.set_speed(square::size);

  m_dst_indication.initialize(rectangle(0,48,g_screen_width-24,g_screen_height-48-24));

  m_dst_indication.set_callback(dst_callback,this);

  m_dst_indication.set_speed(square::size);


  m_src_square_cursor = spaces::image_object(gbstd::g_misc_image,rectangle(24,0,24,24),point());
  m_dst_square_cursor = spaces::image_object(gbstd::g_misc_image,rectangle(24,0,24,24),point());


  m_src_indication.hide_hand_cursor();
  m_dst_indication.hide_hand_cursor();

  m_src_square_cursor.show();


  m_lady_object = spaces::image_object(characters::g_image,g_entries[5].m_image_rectangle,point(0,-48));
  m_boy_object  = spaces::image_object(characters::g_image,g_entries[6].m_image_rectangle,point(0,-48));


  auto&  stg = g_stage_table[g_stage_index];

    for(int  y = 0;  y < stg.get_height();  ++y){
    for(int  x = 0;  x < stg.get_width() ;  ++x){
      auto&  pr = stg.get_prop(x,y);

      g_board.get_square(x,y).set_data(stages::g_square_data_set[pr.get_block_index()]);

      point  pt(square::size*x,square::size*y);

      auto&  o = m_object_table[g_board_width*y+x];

        switch(pr.get_object_index())
        {
      case(1):
          g_object_space.append(m_lady_object);
          break;
      case(2):
          g_object_space.append(m_boy_object);
          break;
      case(3):
          o = spaces::image_object(characters::g_image,g_entries[7].m_image_rectangle,point());

          o.set_base_point(pt);

          g_object_space.append(o);
          break;
      case(4):
          pt.y -= 24;

          o = spaces::image_object(characters::g_image,g_entries[8].m_image_rectangle,point());

          o.set_base_point(pt);

          g_object_space.append(o);
          break;
        }
    }}

   
  auto  lady_pt = stg.get_lady_point();
  auto   boy_pt = stg.get_boy_point();

  m_lady_object.set_base_point(point(square::size*lady_pt.x,square::size*lady_pt.y+24));
  m_boy_object.set_base_point(point(square::size*boy_pt.x,square::size*boy_pt.y   +24));

  g_object_space.append(m_lady_object);
  g_object_space.append(m_boy_object);


  gbstd::g_screen_object_space.append(m_src_square_cursor);
  gbstd::g_screen_object_space.append(m_dst_square_cursor);
  gbstd::g_screen_object_space.append(m_entry_display);
 
}


void
edit::
clean() noexcept
{
  m_src_indication.clean();
  m_dst_indication.clean();

  gbstd::g_screen_object_space.remove_all();
  g_object_space.remove_all();
}


void
edit::
step() noexcept
{
    switch(get_pc())
    {
  case(0):
      initialize();

      set_pc(1);
      break;
  case(1):
        if(!gbstd::g_input.test_p_button())
        {
          m_dst_square_cursor.hide();

          call(m_src_indication);

          set_pc(2);
        }
      break;
  case(2):
        if(!gbstd::g_input.test_p_button() &&
           !gbstd::g_input.test_n_button())
        {
            if(get_end_value().get_integer())
            {
              m_dst_square_cursor.show();

              call(m_dst_indication);

              set_pc(3);
            }

          else
            {
              end();
            }
        }
      break;
  case(3):
        if(!gbstd::g_input.test_n_button())
        {
          set_pc(1);
        }
      break;
    }
}




}}




