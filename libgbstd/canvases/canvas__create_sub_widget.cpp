#include"libgbstd/canvas.hpp"



namespace gbstd{
namespace canvases{


using namespace widgets;


widget*
canvas::
create_operation_widget() noexcept
{
  auto  undo_btn = new button(new label(u"Undo"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->undo();
      }
  });


  auto  copy_btn = new button(new label(u"Copy"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->take_copy();
      }
  });


/*
  auto  shu_btn = new button(new label(u"Shift ↑"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_up(false);
      }
  });


  auto  shl_btn = new button(new label(u"Shift ←"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_left(false);
      }
  });


  auto  shr_btn =   new button(new label(u"Shift →"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_right(false);
      }
  });


  auto  shd_btn =   new button(new label(u"Shift ↓"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_down(false);
      }
  });
*/


  auto  rou_btn = new button(new label(u"Rotate ↑"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_up(true);
      }
  });


  auto  rol_btn = new button(new label(u"Rotate ←"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_left(true);
      }
  });


  auto  ror_btn = new button(new label(u"Rotate →"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_right(true);
      }
  });


  auto  rod_btn = new button(new label(u"Rotate ↓"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->shift_down(true);
      }
  });




  auto  rvl_btn = new button(new label(u"Revolve →"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->revolve();
      }
  });

  auto  rvh_btn = new button(new label(u"Reverse -"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->reverse_horizontally();
      }
  });

  auto  rvv_btn = new button(new label(u"Reverse |"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->reverse_vertically();
      }
  });

  auto  mir_btn = new button(new label(u"Mirror |"),[](button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->mirror_vertically();
      }
  });





/*
  shu_btn->set_userdata(this);
  shl_btn->set_userdata(this);
  shr_btn->set_userdata(this);
  shd_btn->set_userdata(this);
*/
  rou_btn->set_userdata(this);
  rol_btn->set_userdata(this);
  ror_btn->set_userdata(this);
  rod_btn->set_userdata(this);
  undo_btn->set_userdata(this);
  copy_btn->set_userdata(this);
  rvl_btn->set_userdata(this);
  rvh_btn->set_userdata(this);
  rvv_btn->set_userdata(this);
  mir_btn->set_userdata(this);

//  auto  sh_col = new table_column({shu_btn,shl_btn,shr_btn,shd_btn});
  auto  ro_col = new table_column({rou_btn,rol_btn,ror_btn,rod_btn});
  auto  ed_row = new table_row({undo_btn,copy_btn});
  auto  tr_col = new table_column({rvl_btn,rvh_btn,rvv_btn,mir_btn});

  auto  row = new table_row({/*sh_col,*/ro_col,tr_col});

  return new widgets::table_column({row,ed_row});
}


widget*
canvas::
create_tool_widget() noexcept
{
  auto  cb = [](radio_button&  btn, uint32_t  old_state, uint32_t  new_state){
    auto  cv = reinterpret_cast<canvas*>(btn.get_userdata());

    cv->cancel_drawing();

      switch(new_state)
      {
    case(0x01): cv->change_mode_to_draw_dot();break;
    case(0x02): cv->change_mode_to_draw_line();break;
    case(0x04): cv->change_mode_to_draw_rectangle();break;
    case(0x08): cv->change_mode_to_fill_rectangle();break;
    case(0x10): cv->change_mode_to_fill_area();break;
    case(0x20): cv->change_mode_to_select();break;
    case(0x40): cv->change_mode_to_paste();break;
    case(0x80): cv->change_mode_to_layer();break;
      }
  };


  auto  menu = create_radio_menu({new label(u"draw dot"),
                                  new label(u"draw line"),
                                  new label(u"draw rectangle"),
                                  new label(u"fill rectangle"),
                                  new label(u"fill area"),
                                  new label(u"select"),
                                  new label(u"paste"),
                                  new label(u"layer"),
                                  },cb,1,this);


  return menu;
}




}}




