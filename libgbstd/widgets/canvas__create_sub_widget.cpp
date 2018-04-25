#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




widget*
canvas::
create_color_maker() noexcept
{
  auto  cm = new color_maker([](color_maker&  cm, images::color  color){
    reinterpret_cast<canvas*>(cm.get_userdata())->set_drawing_color(color);
  });


  cm->set_userdata(this);

  return cm;  
}


widget*
canvas::
create_tool_widget() noexcept
{
  auto  undo_btn = new button(new label(u"Undo"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        reinterpret_cast<canvas*>(btn.get_userdata())->undo();
      }
  });


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


  shu_btn->set_userdata(this);
  shl_btn->set_userdata(this);
  shr_btn->set_userdata(this);
  shd_btn->set_userdata(this);
  undo_btn->set_userdata(this);

  return new widgets::table_column({shu_btn,shl_btn,shr_btn,shd_btn,undo_btn});
}


widget*
canvas::
create_operation_widget() noexcept
{
  auto  cb = [](radio_button&  btn, uint32_t  old_state, uint32_t  new_state){
    auto  cv = reinterpret_cast<canvas*>(btn.get_userdata());
      switch(new_state)
      {
    case(0x01): cv->change_mode_to_draw_dot();break;
    case(0x02): cv->change_mode_to_draw_line();break;
    case(0x04): cv->change_mode_to_draw_rectangle();break;
    case(0x08): cv->change_mode_to_fill_rectangle();break;
    case(0x10): cv->change_mode_to_fill_area();break;
      }
  };


  auto  menu = create_radio_menu({new label(u"draw dot"),
                                  new label(u"draw line"),
                                  new label(u"draw rectangle"),
                                  new label(u"fill rectangle"),
                                  new label(u"fill area"),
                                  },cb,1,this);


  return menu;
}




}}




